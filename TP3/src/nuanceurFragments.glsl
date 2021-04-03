#version 410

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient[3];
    vec4 diffuse[3];
    vec4 specular[3];
    vec4 position[3];      // dans le repère du monde
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
    vec4 ambient;       // couleur ambiante globale
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

layout (std140) uniform varsUnif
{
    // partie 1: illumination
    int typeIllumination;     // 0:Gouraud, 1:Phong
    bool utiliseBlinn;        // indique si on veut utiliser modèle spéculaire de Blinn ou Phong
    bool afficheNormales;     // indique si on utilise les normales comme couleurs (utile pour le débogage)
    // partie 2: texture
    int iTexCoul;             // numéro de la texture de couleurs appliquée
    // partie 4: texture
    int iTexNorm;             // numéro de la texture de normales appliquée
    // partie 3
    int tesselation;
};

uniform sampler2D laTextureCoul;
uniform sampler2D laTextureNorm;

/////////////////////////////////////////////////////////////////

in Attribs {
    vec4 couleur;
    vec3 normale;
    vec3 ObsVec;
    vec3 LumiVec[3];
    vec2 TexCoord;
} AttribsIn;

out vec4 FragColor;

float attenuation = 1.0;
vec4 calculerReflexion(in int j, in vec3 L, in vec3 N, in vec3 O) // pour la lumière j
{
    float NdotL = max(0.0, dot(N, L));

    vec4 frontColor = FrontMaterial.ambient * LightSource.ambient[j];
    float NdotHV;
    if (NdotL > 0) {
        if (utiliseBlinn) {
            NdotHV = pow(max(0.0, dot(normalize(L + O), N)), FrontMaterial.shininess);
        }
        else {
            NdotHV = pow(max(0.0, dot(reflect(-L, N), O)), FrontMaterial.shininess);
        }
        frontColor += FrontMaterial.diffuse * LightSource.diffuse[j] * NdotL + FrontMaterial.specular * LightSource.specular[j] * NdotHV;
    }
    return frontColor;
}

void main( void )
{
    vec3 normale = gl_FrontFacing ? AttribsIn.normale : -AttribsIn.normale;

    vec4 textureCouleur = texture(laTextureCoul, AttribsIn.TexCoord);

    if (typeIllumination == 1) {
        // Phong
        FragColor = FrontMaterial.ambient * LightModel.ambient + FrontMaterial.emission;

        for (int j = 0; j < 3; j++) {
            FragColor += calculerReflexion(j, AttribsIn.LumiVec[j], normale, AttribsIn.ObsVec);
        }

        if (iTexCoul != 0) {
            FragColor = FragColor * textureCouleur;
            if (length(textureCouleur.rgb) < 0.5) discard;
        }

        FragColor = clamp(FragColor, 0.0, 1.0);
    }
    else {
        if (iTexCoul != 0) {
            FragColor = clamp(AttribsIn.couleur * textureCouleur, 0.0, 1.0);
            if (length(textureCouleur.rgb) < 0.5) discard;
        }
        else {
            FragColor = AttribsIn.couleur;
        }
    }
}
