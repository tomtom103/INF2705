#version 410

////////////////////////////////////////////////////////////////////////////////

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;      // dans le repère du monde
    vec3 spotDirection; // dans le repère du monde
    float spotExponent;
    float spotAngleOuverture; // ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource;

// Définition des paramètres des matériaux
layout (std140) uniform MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} FrontMaterial;

// Définition des paramètres globaux du modèle de lumière
layout (std140) uniform LightModelParameters
{
    vec4 ambient;       // couleur ambiante
    bool localViewer;   // observateur local ou à l'infini?
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

////////////////////////////////////////////////////////////////////////////////

uniform int illumination; // on veut calculer l'illumination ?

const bool utiliseBlinn = false;

in Attribs {
    vec4 couleur;
    vec3 lumiDir;
    vec3 obsVec;
    vec3 normale;
} AttribsIn;

out vec4 FragColor;

float attenuation = 1.0;

vec4 reflexionBlinnPhong( in vec3 L, in vec3 N, in vec3 O )
{

    vec4 coul = vec4(0.0);

    coul += FrontMaterial.ambient * LightSource.ambient;

    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calcul de la composante diffuse
        coul += attenuation * AttribsIn.couleur * LightSource.diffuse * NdotL;

        // calcul de la composante spéculaire (Blinn ou Phong : spec = BdotN ou RdotO )
        float spec = max( 0.0, ( utiliseBlinn ) ?
                          dot( normalize( L + O ), N ) : // dot( B, N )
                          dot( reflect( -L, N ), O ) ); // dot( R, O )
        if ( spec > 0 ) coul += FrontMaterial.specular * LightSource.specular * pow( spec, FrontMaterial.shininess );
    }


    return( coul );
}

void main( void )
{
    // la couleur du fragment est la couleur interpolée
    vec3 lumiDir = normalize(AttribsIn.lumiDir);
    vec3 obsVec = normalize(AttribsIn.obsVec);
    vec3 normale = normalize(gl_FrontFacing ? AttribsIn.normale : - AttribsIn.normale);

    vec4 coul;

    if(illumination == 1) {
        coul = reflexionBlinnPhong(lumiDir, normale, obsVec);
    }
    else {
        coul = AttribsIn.couleur;
    }
    // seuiller chaque composante entre 0 et 1 et assigner la couleur finale du fragmen
    FragColor = clamp(coul, 0.0, 1.0);
}