#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matrProj;

uniform int texnumero;
uniform float tempsDeVieMax;

in Attribs {
    vec4 couleur;
    float tempsDeVieRestant;
    //float sens; // du vol (partie 3)
    //float hauteur; // du vol (partie 3)
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec2 texcoord;  
} AttribsOut;

// la hauteur minimale en-dessous de laquelle les lutins ne tournent plus (partie 3)
const float hauteurInerte = 8.0;

void main()
{
    vec2 coins[4];
    coins[0] = vec2(-0.5, 0.5);
    coins[1] = vec2(-0.5, -0.5);
    coins[2] = vec2(0.5, 0.5);
    coins[3] = vec2(0.5, -0.5);

    for(int i = 0; i < 4; i++) {
        vec2 decalage;

        if (texnumero == 1) {
            // DVD
            float angle = 6.0 * AttribsIn[0].tempsDeVieRestant;
            decalage = mat2(cos(angle), sin(angle), -sin(angle), cos(angle)) * coins[i];
        }
        else {
            decalage = coins[i];
        }
        vec4 pos = vec4(gl_in[0].gl_Position.xy + gl_in[0].gl_PointSize * decalage, gl_in[0].gl_Position.zw);

        gl_Position = matrProj * pos;

        AttribsOut.couleur = AttribsIn[0].couleur;

        if (AttribsIn[0].tempsDeVieRestant < 5)
            AttribsOut.couleur.a = clamp(0.5 * AttribsIn[0].tempsDeVieRestant, 0, 1);

        AttribsOut.texcoord = coins[i] + vec2(0.5, 0.5);
        if (texnumero == 2) {
            // Mario
            const float nlutins = 12.0;
            int num = int(mod(18.0 * AttribsIn[0].tempsDeVieRestant, nlutins));
            AttribsOut.texcoord.x = (AttribsOut.texcoord.x + num) / nlutins;
        }

        EmitVertex();
    }
}
