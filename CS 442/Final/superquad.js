

var superQuad = {

    N : 64,
    M : 64,
    radius : 5,

    //bulge factors
    n : 2,
    m : 2,

    verts : -1,
    normals : -1,
    textureCoords : -1,
    triangleStrip : -1,    
    lines: -1,

    setData : function() {
        this.createVarray();
        this.createNonUniformTexture();  
        this.createTriangleStrips();
        this.lines = this.createWireFrame(this.triangleStrip);
        },             

    createVarray : function() {

        var N = this.N; 
        var M = this.M;
        var n = this.n;
        var m = this.m;
        var radius = this.radius;

        var x = function(u,v){
            return Math.cos(v) * Math.pow(Math.abs(Math.cos(v)), 2/m-1) * Math.cos(u) * Math.pow(Math.abs(Math.cos(u)), 2/n-1);
        }
        var y = function(u,v){
            return Math.cos(v) * Math.pow(Math.abs(Math.cos(v)), 2/m-1) * Math.sin(u) * Math.pow(Math.abs(Math.sin(u)), 2/n-1);
        }
        var z = function(u,v){
            return Math.sin(v) * Math.pow(Math.abs(Math.sin(v)), 2/m-1);
        }

        var Nx = function(u,v){
            return Math.cos(v) * Math.pow(Math.abs(Math.cos(v)), 1-2/m) * Math.cos(u) * Math.pow(Math.abs(Math.cos(u)), 1-2/n);
        }

        var Ny = function(u,v){ 
            return Math.cos(v) * Math.pow(Math.abs(Math.cos(v)), 1-2/m) * Math.sin(u) * Math.pow(Math.abs(Math.sin(u)), 1-2/n);
        }

        var Nz = function(u,v){
            return Math.sin(v) * Math.pow(Math.abs(Math.sin(v)), 1-2/m);
        }

         var index = function(i,j) {return i*(M+1) + j;}

        var meh = 0; 
        var teh = 0;

        var normals = new Float32Array((N+1)*(M+1) * 3);
        var verts = new Float32Array((N+1)*(M+1) * 3);

        for (var i = 0, v = -Math.PI/2; i <= N; i++, v += Math.PI/N) {
            //if (i == N) v = Math.PI/2;
            for(var j = 0, u = -Math.PI; j <= M; j++, u += 2*Math.PI/M) {
                //if (j == M) u = Math.PI;
                var tempV = [x(u,v), y(u,v), z(u,v)];
                var tempN = [Nx(u,v), Ny(u,v), Nz(u,v)];
                for (var k = 0; k < 3; k++){
                    verts[meh] = tempV[k] * radius;
                    normals[meh] = tempN[k] * radius;
                    meh++;
                }
            }
        }
        this.normals = normals;
        this.verts = verts;
    },

    createNonUniformTexture : function(){


        var N = this.N; 
        var M = this.M;
        //var n = this.n;
        //var m = this.m;
        var verts = this.verts;
        var TextureCoords = new Float32Array((N+1)*(M+1) * 2);
        var numerator = 0, denominator = 0;
        var tempA = 0; 
        var tempB =  0;
        var euclidS = new Float32Array(M+1);
        var euclidT = new Float32Array(N+1);

        var index = function(i,j) {return i*(M+1) + j;}

        var mag = function(vec){
            return Math.sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        }

        //calculate s values
        for (var i = 1; i < N; i++){
            denominator = 0;
            for (var j = 1; j <= M; j++){  
            tempA = 3 * index(i,j);                  
                var currentV = [verts[tempA], verts[tempA+1], verts[tempA+2]];
                var prevV = [verts[tempA-3], verts[tempA-2], verts[tempA-1]];
                euclidS[j] = mag([currentV[0]-prevV[0],currentV[1]-prevV[1],currentV[2]-prevV[2]]);
                denominator += euclidS[j];
            }
            for (var j = 1; j <= M; j++){
                tempB = 2 * index(i,j);
                numerator = 0;
                for (var k = 1; k <= j; k++){
                    numerator += euclidS[k];
                }
                TextureCoords[tempB] = numerator/denominator;
            }
        }

        //calculate t values
        for (var j = 0; j <= M; j++){
            denominator = 0;               
            for (var i = 1; i <= N; i++){
                tempA = 3 * index(i,j);
                var currentV = [verts[tempA], verts[tempA+1], verts[tempA+2]];
                tempA = 3 * index(i-1, j);
                var prevV = [verts[tempA], verts[tempA+1], verts[tempA+2]];
                euclidT[i] = mag([currentV[0]-prevV[0],currentV[1]-prevV[1],currentV[2]-prevV[2]]);
                denominator += euclidT[i];
            }
            for (var i = 1; i <= N; i++){
                tempB = 2 * index(i,j) + 1;
                numerator = 0;
                for (var k = 1; k <= i; k++){
                    numerator += euclidT[k];
                }
                TextureCoords[tempB] = numerator/denominator;
            }
        }

        //fill edge cases
        for (var j = 1; j <= M; j++){
            TextureCoords[index(0, j)*2] = TextureCoords[index(1,j)*2];
            TextureCoords[index(N, j)*2] = TextureCoords[index(N-1,j)*2];
        }

        this.textureCoords = TextureCoords;      
    },


    //create triangle strips
    createTriangleStrips : function(){
        var N = this.N;
        var M = this.M;
        var index = function(i,j) {return i*(M+1) + j;}
        var meh = 0;
        var numIndices = N * (2 * (M + 1) + 2) - 2;
        var triangleStrip = new Uint16Array(numIndices);

        for (var i = 0; i < N; i++){
            if (i > 0) triangleStrip[meh++] = index(i, 0);
            for (var j = 0; j <= M; j++){
                triangleStrip[meh++] = index(i, j);
                triangleStrip[meh++] = index(i+1, j);
            }
            if (i < N-1) triangleStrip[meh++] = index(i+1, M);
        }
        this.triangleStrip = triangleStrip;
    },

    createWireFrame : function(triangleStrip){
            var lines = [];
            triangleStrip = this.triangleStrip;
            lines.push(triangleStrip[0], triangleStrip[1]);
            for (var i = 2; i < triangleStrip.length; i++){
                var a = triangleStrip[i-2];
                var b = triangleStrip[i-1];
                var c = triangleStrip[i];
                if (a != b && b != c && a != c) lines.push(a, c, b, c);
            }
            return new Uint16Array(lines);
        }

};