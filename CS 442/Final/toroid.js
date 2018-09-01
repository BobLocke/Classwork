
var toroid = {


    bigRadius : 10, // a
    smallRadius : 2, // b
    bigLoops : 3, // p
    smallLoops : 8, // q
     
    N : 400,
    M : 80,
    wfRadius : .5,

                  

    // wireframe
    verts : -1,
    normals : -1,
    textureCoords: -1,
    triangleStrip : -1,
    lines : -1,
    texScaleX : 0, 
    texScaleY : 0,

    createArrays : function(){

        var a = this.bigRadius;
        var b = this.smallRadius;
        var p = this.bigLoops;
        var q = this.smallLoops;
        var N = this.N;
        var M = this.M;
        var wfRadius = this.wfRadius;
        var dt = 2 * Math.PI/N, du = 2*Math.PI/M;
        var n = 0;


        var sinp = function(t) {return Math.sin(p * t);}
        var sinq = function(t) {return Math.sin(q * t);}
        var cosp = function(t) {return Math.cos(p * t);}
        var cosq = function(t) {return Math.cos(q * t);}

        var x = function(t){
            return (a + b * cosq(t)) * cosp(t); 
        }
        var y = function(t){
            return (a + b * cosq(t)) * sinp(t);
        }
        var z = function(t){
            return b * sinq(t);
        }

        var dx = function(t){
            return -p * y(t) - b * q * sinq(t) * cosp(t);
        }

        var dy = function(t){ 
            return p * x(t) - b * q * sinq(t) * sinp(t);
        }

        var dz = function(t){
           return b * q * cosq(t);
        }

        var ddx = function(t){
            return -p * dy(t) + q * b * (p * sinq(t) * sinp(t) - q * cosq(t) * cosp(t));
        }

        var ddy = function(t){
            return p * dx(t) - q * b * (p * sinq(t) * cosp(t) + q * cosq(t) * sinp(t));
        }
        var ddz = function(t){
            return -q * q * b * sinq(t);
        }

        var index = function(i,j) {return i*(M+1) + j;}

        var teh = 0;
        var verts = new Float32Array((N+1)*(M+1)*3);
        var normals = new Float32Array((N+1)*(M+1)*3);
        var numIndices = (2*(M + 2)* N - 2);
        var triangleStrip = new Uint16Array(numIndices);
        var textureCoords = new Float32Array((N+1)*(M+1)*2);





        for (var i = 0, t = 0.0; i <= N; i++, t += dt){
            if (i == N) t = 0.0;
            var C = [x(t), y(t), z(t)];
            var T = [dx(t), dy(t), dz(t)];
            var A = [ddx(t), ddy(t), ddz(t)];
            var B = cross3(T, A);
            norm3(T);
            norm3(B);
            var N_ = cross3(B, T);
            for(var j = 0, u = 0.0; j <= M; j++, u += du) {
                if (j == M) u = 0.0;
                textureCoords[teh++] = (i < N) ? i/N : 1.0;
                textureCoords[teh++] = (j < M) ? j/M : 1.0;
                var cosu = Math.cos(u), sinu = Math.sin(u);
                for (var k = 0; k < 3; k++){
                    normals[n] = cosu * N_[k] + sinu*B[k];
                    verts[n] = C[k] + wfRadius * normals[n];
                    n++;
                }
            }
        }

        n = 0;
        for (var i = 0; i < N; i++){
            if (i > 0) triangleStrip[n++] = index(i, 0);
            for (var j = 0; j <= M; j++){
                triangleStrip[n++] = index(i, j);
                triangleStrip[n++] = index(i+1, j);
            }
            if (i < N-1) triangleStrip[n++] = index(i+1, M);
        }

        this.triangleStrip = triangleStrip;
        this.textureCoords = textureCoords;
        this.verts = verts;
        this.normals = normals;

        this.texScaleX = 2 * Math.PI * p * a;
        this.texScaleY = 2 * Math.PI * wfRadius;
    }
};