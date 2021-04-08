#version 410

uniform sampler2D leLutin;
uniform int texnumero;

in Attribs {
    vec4 couleur;
    vec2 texcoord;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    vec4 coulTexture = texture( leLutin, AttribsIn.texcoord );

    if (texnumero != 0 && coulTexture.a < 0.1) {
        discard;
    }
    else if (texnumero != 0) {
        FragColor = vec4(mix(AttribsIn.couleur.rgb, coulTexture.rgb, 0.6), AttribsIn.couleur.a);
    }
    else {
        FragColor = AttribsIn.couleur;
    }
}
