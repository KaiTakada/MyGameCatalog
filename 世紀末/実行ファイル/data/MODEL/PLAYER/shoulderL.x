xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 74;
 -0.37135;1.08803;-1.34848;,
 -0.37135;-0.26045;-1.90704;,
 -0.98665;0.09337;-2.56141;,
 -0.98665;1.90456;-1.81119;,
 -0.37135;-1.60893;-1.34848;,
 -0.98665;-2.34430;-1.81119;,
 -0.37135;-2.16749;0.00000;,
 -0.98665;-3.09453;0.00000;,
 -0.37135;-1.60893;1.34848;,
 -0.98665;-2.34430;1.81119;,
 -0.37135;-0.26045;1.90704;,
 -0.98665;0.09337;2.56141;,
 -0.37135;1.08803;1.34848;,
 -0.98665;1.90456;1.81119;,
 -0.37135;1.64659;0.00000;,
 -0.98665;2.65478;0.00000;,
 -1.78283;0.03478;-3.30948;,
 -1.78283;2.37494;-2.34015;,
 -1.78283;-2.84618;-2.34015;,
 -1.78283;-3.81550;0.00000;,
 -1.78283;-2.84618;2.34015;,
 -1.78283;0.03478;3.30948;,
 -1.78283;2.37494;2.34015;,
 -1.78283;3.34426;0.00000;,
 -2.79558;-0.16153;-4.11934;,
 -2.79558;2.75128;-2.91281;,
 -2.79558;-3.07434;-2.91281;,
 -2.79558;-4.28087;0.00000;,
 -2.79558;-3.07434;2.91281;,
 -2.79558;-0.16153;4.11934;,
 -2.79558;2.75128;2.91281;,
 -2.79558;3.95781;0.00000;,
 -9.90368;-0.16153;-4.11934;,
 -9.90368;2.75128;-2.91281;,
 -9.90368;-3.07434;-2.91281;,
 -9.90368;-4.28087;0.00000;,
 -9.90368;-3.07434;2.91281;,
 -9.90368;-0.16153;4.11934;,
 -9.90368;2.75128;2.91281;,
 -9.90368;3.95781;0.00000;,
 -17.01178;-0.16153;-4.11934;,
 -17.01178;2.75128;-2.91281;,
 -17.01178;-3.07434;-2.91281;,
 -17.01178;-4.28087;0.00000;,
 -17.01178;-3.07434;2.91281;,
 -17.01178;-0.16153;4.11934;,
 -17.01178;2.75128;2.91281;,
 -17.01178;3.95781;0.00000;,
 -18.02453;0.03478;-3.30948;,
 -18.02453;2.37494;-2.34015;,
 -18.02453;-2.84618;-2.34015;,
 -18.02453;-3.81550;0.00000;,
 -18.02453;-2.84618;2.34015;,
 -18.02453;0.03478;3.30948;,
 -18.02453;2.37494;2.34015;,
 -18.02453;3.34426;0.00000;,
 -18.82071;0.09337;-2.56141;,
 -18.82071;1.90456;-1.81119;,
 -18.82071;-2.34430;-1.81119;,
 -18.82071;-3.09453;0.00000;,
 -18.82071;-2.34430;1.81119;,
 -18.82071;0.09337;2.56141;,
 -18.82071;1.90456;1.81119;,
 -18.82071;2.65478;0.00000;,
 -19.43601;-0.26045;-1.90704;,
 -19.43601;1.08803;-1.34848;,
 -19.43601;-1.60893;-1.34848;,
 -19.43601;-2.16749;0.00000;,
 -19.43601;-1.60893;1.34848;,
 -19.43601;-0.26045;1.90704;,
 -19.43601;1.08803;1.34848;,
 -19.43601;1.64659;0.00000;,
 -0.37135;-0.26045;0.00000;,
 -19.43601;-0.26045;0.00000;;
 
 80;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 4;3,2,16,17;,
 4;2,5,18,16;,
 4;5,7,19,18;,
 4;7,9,20,19;,
 4;9,11,21,20;,
 4;11,13,22,21;,
 4;13,15,23,22;,
 4;15,3,17,23;,
 4;17,16,24,25;,
 4;16,18,26,24;,
 4;18,19,27,26;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,17,25,31;,
 4;25,24,32,33;,
 4;24,26,34,32;,
 4;26,27,35,34;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,25,33,39;,
 4;33,32,40,41;,
 4;32,34,42,40;,
 4;34,35,43,42;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;38,39,47,46;,
 4;39,33,41,47;,
 4;41,40,48,49;,
 4;40,42,50,48;,
 4;42,43,51,50;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 4;46,47,55,54;,
 4;47,41,49,55;,
 4;49,48,56,57;,
 4;48,50,58,56;,
 4;50,51,59,58;,
 4;51,52,60,59;,
 4;52,53,61,60;,
 4;53,54,62,61;,
 4;54,55,63,62;,
 4;55,49,57,63;,
 4;57,56,64,65;,
 4;56,58,66,64;,
 4;58,59,67,66;,
 4;59,60,68,67;,
 4;60,61,69,68;,
 4;61,62,70,69;,
 4;62,63,71,70;,
 4;63,57,65,71;,
 3;1,0,72;,
 3;4,1,72;,
 3;6,4,72;,
 3;8,6,72;,
 3;10,8,72;,
 3;12,10,72;,
 3;14,12,72;,
 3;0,14,72;,
 3;65,64,73;,
 3;64,66,73;,
 3;66,67,73;,
 3;67,68,73;,
 3;68,69,73;,
 3;69,70,73;,
 3;70,71,73;,
 3;71,65,73;;
 
 MeshMaterialList {
  1;
  80;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\PLAYER\\human_body_use.png";   }
  }
 }
 MeshNormals {
  74;
  0.956393;0.200102;-0.212769;,
  0.939982;0.001682;-0.341220;,
  0.947688;-0.209600;-0.240738;,
  0.958493;-0.285115;-0.000000;,
  0.947688;-0.209600;0.240738;,
  0.939982;0.001682;0.341220;,
  0.956393;0.200102;0.212769;,
  0.963562;0.267485;0.000000;,
  0.750412;0.462179;-0.472517;,
  0.718442;0.017683;-0.695362;,
  0.739435;-0.450995;-0.499839;,
  0.758865;-0.651248;-0.000000;,
  0.739435;-0.450995;0.499839;,
  0.718442;0.017683;0.695362;,
  0.750412;0.462179;0.472517;,
  0.764419;0.644720;0.000000;,
  0.609477;0.564502;-0.556665;,
  0.634695;0.024867;-0.772363;,
  0.606484;-0.555182;-0.569166;,
  0.552142;-0.833750;-0.000000;,
  0.606484;-0.555182;0.569166;,
  0.634695;0.024867;0.772363;,
  0.609477;0.564502;0.556665;,
  0.588918;0.808193;0.000000;,
  0.291165;0.679471;-0.673455;,
  0.316873;0.009313;-0.948422;,
  0.270629;-0.683259;-0.678172;,
  0.215087;-0.976595;-0.000000;,
  0.270629;-0.683259;0.678172;,
  0.316873;0.009313;0.948422;,
  0.291165;0.679471;0.673455;,
  0.271687;0.962386;0.000000;,
  0.000000;0.707107;-0.707107;,
  0.000000;-0.000000;-1.000000;,
  0.000000;-0.707107;-0.707107;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.707107;0.707107;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.707107;0.707107;,
  0.000000;1.000000;0.000000;,
  -0.291165;0.679471;-0.673455;,
  -0.316873;0.009313;-0.948422;,
  -0.270629;-0.683259;-0.678172;,
  -0.215087;-0.976595;0.000000;,
  -0.270629;-0.683259;0.678172;,
  -0.316873;0.009313;0.948422;,
  -0.291165;0.679471;0.673455;,
  -0.271687;0.962386;0.000000;,
  -0.609476;0.564502;-0.556665;,
  -0.634695;0.024867;-0.772363;,
  -0.606484;-0.555182;-0.569166;,
  -0.552142;-0.833750;0.000000;,
  -0.606484;-0.555182;0.569166;,
  -0.634695;0.024867;0.772363;,
  -0.609476;0.564502;0.556665;,
  -0.588918;0.808193;0.000000;,
  -0.750412;0.462179;-0.472517;,
  -0.718442;0.017683;-0.695362;,
  -0.739435;-0.450995;-0.499839;,
  -0.758865;-0.651248;0.000000;,
  -0.739435;-0.450995;0.499839;,
  -0.718442;0.017683;0.695362;,
  -0.750412;0.462179;0.472517;,
  -0.764419;0.644720;0.000000;,
  -0.956393;0.200102;-0.212769;,
  -0.939982;0.001682;-0.341220;,
  -0.947688;-0.209600;-0.240738;,
  -0.958493;-0.285115;0.000000;,
  -0.947688;-0.209600;0.240738;,
  -0.939982;0.001682;0.341220;,
  -0.956393;0.200102;0.212769;,
  -0.963562;0.267485;0.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;;
  80;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,32,40,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,40,48,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,48,56,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,56,64,71;,
  3;1,0,72;,
  3;2,1,72;,
  3;3,2,72;,
  3;4,3,72;,
  3;5,4,72;,
  3;6,5,72;,
  3;7,6,72;,
  3;0,7,72;,
  3;64,65,73;,
  3;65,66,73;,
  3;66,67,73;,
  3;67,68,73;,
  3;68,69,73;,
  3;69,70,73;,
  3;70,71,73;,
  3;71,64,73;;
 }
 MeshTextureCoords {
  74;
  0.086149;0.928341;,
  0.086257;0.940837;,
  0.091710;0.938703;,
  0.091561;0.921883;,
  0.086213;0.951669;,
  0.091640;0.958687;,
  0.086045;0.954458;,
  0.091380;0.962385;,
  0.085851;0.947671;,
  0.091094;0.953250;,
  0.085745;0.935315;,
  0.090936;0.931334;,
  0.085787;0.924530;,
  0.091014;0.916842;,
  0.085954;0.921600;,
  0.091272;0.912853;,
  0.098793;0.940280;,
  0.098595;0.918502;,
  0.098656;0.963795;,
  0.098262;0.968528;,
  0.097850;0.956712;,
  0.097650;0.930750;,
  0.097785;0.912046;,
  0.098176;0.906849;,
  0.107820;0.943150;,
  0.107565;0.915982;,
  0.107582;0.966610;,
  0.106999;0.972469;,
  0.106419;0.957761;,
  0.106171;0.931247;,
  0.106395;0.908004;,
  0.106971;0.901491;,
  0.170119;0.942892;,
  0.169803;0.915740;,
  0.169467;0.966341;,
  0.168249;0.972200;,
  0.167187;0.957502;,
  0.166881;0.931003;,
  0.167504;0.907771;,
  0.168711;0.901259;,
  0.232350;0.942635;,
  0.231973;0.915498;,
  0.231284;0.966073;,
  0.229433;0.971931;,
  0.227889;0.957244;,
  0.227526;0.930759;,
  0.228546;0.907537;,
  0.230384;0.901027;,
  0.240728;0.939697;,
  0.240419;0.917947;,
  0.239760;0.963186;,
  0.238202;0.967918;,
  0.236904;0.956121;,
  0.236670;0.930191;,
  0.237539;0.911507;,
  0.239089;0.906313;,
  0.247200;0.938067;,
  0.246958;0.921270;,
  0.246396;0.958026;,
  0.245147;0.961724;,
  0.244106;0.952605;,
  0.243949;0.930718;,
  0.244652;0.916243;,
  0.245897;0.912256;,
  0.252077;0.940155;,
  0.251894;0.927678;,
  0.251525;0.950973;,
  0.250570;0.953762;,
  0.249773;0.946988;,
  0.249594;0.934650;,
  0.250135;0.923878;,
  0.251087;0.920950;,
  0.085999;0.938060;,
  0.250828;0.937386;;
 }
}
