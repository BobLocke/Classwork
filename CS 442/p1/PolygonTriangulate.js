var Vertex = function(x, y){
    this.x = x;
    this.y = y;
}

var pointOutside = function(a, b, c, p){
        var v = [];
        v[0] = a;
        v[1] = b;
        v[2] = c;
        var d = 0;
        for (var i = 0; i < 3; i++){
            var u = new Vertex(v[(i+1) % 3].x - v[i].x, v[(i+1) % 3].y - v[i].y);
            var w = new Vertex(p.x - v[i].x, p.y - v[i].y);
            var z = u.x * w.y - u.y * w.x;
            if (i === 0) d = z;
            else if (z * d < 0) return true;
        }
        return false;
    }

var vertices = [];
var inputChunks = [];
var area = 0.0;
var indices = [];
var triangles = [];

process.stdin.setEncoding('utf8');
process.stdin.resume();

process.stdin.on('data', function(input) {
    inputChunks.push(input);
});


process.stdin.on('end', function(){
    var inputJSON = inputChunks.join();
    inputChunks = JSON.parse(inputJSON);
    for (var i = 0; i < inputChunks.length; i++){
       vertices.push(new Vertex(inputChunks[i].x, inputChunks[i].y));

    }
    var vLen = vertices.length;
    if (vertices.length < 3) return [];
    for (var i = 0; i < vLen; i++){
        area += .5 * (vertices[i].x * vertices[(i+1) % (vLen)].y - vertices[(i+1) % (vLen)].x * vertices[i].y);
        indices[i] = i;
    }
    
    //process.stdout.write("indices.length: " + indices.length + " | vertices.length: "+ vertices.length + "\n");

    while (indices.length > 3){
        var a = 0;
        var n = 0;
        var ear;
        var localArea = [];
        var testSum = 0;
        var iLen = indices.length;
        for (var i = 0; i < indices.length; i++){
            localArea[i] = vertices[indices[(i - 1 + iLen) % iLen]].x * vertices[indices[i]].y - vertices[indices[i]].x * vertices[indices[(i - 1 + iLen) % iLen]].y;
            localArea[i] += vertices[indices[i]].x * vertices[indices[(i + 1) % iLen]].y - vertices[indices[(i + 1) % iLen]].x * vertices[indices[i]].y;
            localArea[i] += vertices[indices[(i + 1) % iLen]].x * vertices[indices[((i - 1 + iLen) % iLen) % iLen]].y - vertices[indices[((i - 1 + iLen) % iLen) % iLen]].x * vertices[indices[(i + 1) % iLen]].y;
            localArea[i] *= .5;
            if ((Math.abs(localArea[i]) > a) && (localArea[i] * area > 0)){
                ear = true;
                for (var j = 0; j < indices.length; j++){
                    if ((j === i) || (j === (i+1) % iLen) || (j === (i-1+iLen) % iLen)) continue;
                    if (ear === false) {break;}
                    ear = pointOutside(vertices[indices[(i - 1 + iLen) % iLen]], vertices[indices[i]], vertices[indices[(i+1) % iLen]], vertices[indices[j]]);
                }
                if (ear){
                    n = i;
                    a = Math.abs(localArea[i]);
                }
            }
            //process.stdout.write("LocalArea[i] (" + i + ", " + (i + 1) % vLen + ", " + (i + 2) % vLen + "):" + localArea[i] + "\n");
            //testSum += localArea[i];
        }
        triangles.push(indices[n]);
        triangles.push(indices[(n+1) % iLen]);
        triangles.push(indices[(n-1+iLen) % iLen]);
        indices.splice(n, 1);
        //process.stdout.write("Index removes, new list:" + indices.toString() + "\n");
        

        //testSum *= .5;
        //process.stdout.write("totalArea: " + area + " | testSum (triangle area): " + testSum +  "\n");
    }
    triangles.push(indices[0]);
    triangles.push(indices[1]);
    triangles.push(indices[2]);
    //process.stdout.write("{\n\"verts\" : "+ JSON.stringify(vertices) + ",\n");
    process.stdout.write(JSON.stringify(triangles, null, 4) + "\n");
});