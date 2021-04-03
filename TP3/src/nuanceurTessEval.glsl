#version 410

layout(quads) in;

in Attribs{
    vec4 couleur;
    vec3 normale;
    vec3 ObsVec;
    vec3 LumiVec[3];
    vec2 TexCoord;
} AttribsIn[];

out Attribs{
    vec4 couleur;
    vec3 normale;
    vec3 ObsVec;
    vec3 LumiVec[3];
    vec2 TexCoord;
} AttribsOut;

uniform mat4 matrVisu;
uniform mat4 matrProj;

vec2 interpole(vec2 v0, vec2 v1, vec2 v2, vec2 v3) {
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec2 v01 = mix(v0, v1, gl_TessCoord.x);
    vec2 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}
vec3 interpole(vec3 v0, vec3 v1, vec3 v2, vec3 v3) {
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec3 v01 = mix(v0, v1, gl_TessCoord.x);
    vec3 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}
vec4 interpole(vec4 v0, vec4 v1, vec4 v2, vec4 v3) {
    // mix( x, y, f ) = x * (1-f) + y * f.
    vec4 v01 = mix(v0, v1, gl_TessCoord.x);
    vec4 v32 = mix(v3, v2, gl_TessCoord.x);
    return mix(v01, v32, gl_TessCoord.y);
}


void main()
{
    // À cause de l'ordre de lecture des sommets pour la tesselation en quads, on doit inverser les 2 derniers sommets
    gl_Position = matrProj * matrVisu * (interpole(
        gl_in[0].gl_Position,
        gl_in[1].gl_Position,
        gl_in[3].gl_Position,
        gl_in[2].gl_Position
    ));
    AttribsOut.normale = normalize(interpole(
        AttribsIn[0].normale, 
        AttribsIn[1].normale, 
        AttribsIn[3].normale, 
        AttribsIn[2].normale
    ));

    AttribsOut.TexCoord = interpole(
        AttribsIn[0].TexCoord, 
        AttribsIn[1].TexCoord, 
        AttribsIn[3].TexCoord, 
        AttribsIn[2].TexCoord
    );

    AttribsOut.ObsVec = vec3(0.0);

    for (int i = 0; i < 3; i++) AttribsOut.LumiVec[i] = vec3(0.0);
   
    if (any(lessThan(gl_TessCoord, vec3(0.001))))
        AttribsOut.couleur = interpole(
            AttribsIn[0].couleur,
            AttribsIn[1].couleur,
            AttribsIn[3].couleur, 
            AttribsIn[2].couleur
        );
    else
        AttribsOut.couleur = vec4(1.0);

}