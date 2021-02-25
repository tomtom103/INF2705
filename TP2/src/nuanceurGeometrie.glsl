#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Attribs {
    vec4 couleur;
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec3 lumiDir;
    vec3 obsVec;
    vec3 normale;
} AttribsOut;

void main()
{
    // source de la lumiere
    vec3 lumiDir = vec3( 0, 0, 1 );
    // observateur
    vec3 obsVec = vec3( 0, 0, 1 );

    // calcul de la normale
    vec3 arrete1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
    vec3 arrete2 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
    vec3 normale;
    normale = cross(arrete1, arrete2);

    for( int i = 0 ; i < gl_in.length() ; i++ ) {
        AttribsOut.lumiDir = lumiDir;
        AttribsOut.obsVec = obsVec;
        gl_Position = gl_in[i].gl_Position;
        AttribsOut.couleur = AttribsIn[i].couleur;
        AttribsOut.normale = normale;

        EmitVertex();
    }

    EndPrimitive();
}