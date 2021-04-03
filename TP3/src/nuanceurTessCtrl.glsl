#version 410

layout(vertices = 4) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;

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
} AttribsOut[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0)
    {
        gl_TessLevelInner[0] = TessLevelInner;
        gl_TessLevelInner[1] = TessLevelInner;
        gl_TessLevelOuter[0] = TessLevelOuter;
        gl_TessLevelOuter[1] = TessLevelOuter;
        gl_TessLevelOuter[2] = TessLevelOuter;
        gl_TessLevelOuter[3] = TessLevelOuter;
    }
    AttribsOut[gl_InvocationID].couleur = AttribsIn[gl_InvocationID].couleur;

    AttribsOut[gl_InvocationID].normale = AttribsIn[gl_InvocationID].normale;

    //AttribsOut[gl_InvocationID].ObsVec = AttribsIn[gl_InvocationID].ObsVec;

    //for (int i = 0; i < 3; i++) AttribsOut[gl_InvocationID].LumiVec[i] = AttribsIn[gl_InvocationID].LumiVec[i];

    AttribsOut[gl_InvocationID].TexCoord = AttribsIn[gl_InvocationID].TexCoord;
}