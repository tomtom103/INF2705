#ifndef __OISEAU_H__
#define __OISEAU_H__

// les formes
FormeCube *cube = NULL;
FormeQuad *quad = NULL;
FormeSphere *sphere = NULL;
FormeCylindre *cylindre = NULL;
// (partie 1) Vous devez vous servir des quatre fonctions ci-dessous (*sans les modifier*) pour tracer tous les parties des objets.
// affiche un cylindre de rayon 1.0 et de longueur 1.0, dont la base est centrée en (0,0,0)
void afficherCylindre( ) { cylindre->afficher(); }
// affiche une sphère de rayon 1.0, centrée en (0,0,0)
void afficherSphere( ) { sphere->afficher(); }
// affiche un cube d'arête 1.0, centrée en (0,0,0)
void afficherCube( ) { cube->afficher(); }
// affiche un aile d'arête 1
void afficherQuad( ) { quad->afficher(); }

// affiche la position courante du repère (pour débogage)
void afficherRepereCourant( int num = 0 )
{
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    FenetreTP::afficherAxes( 1.5, 3.0 );
}

// partie 1: l'oiseau
class Oiseau
{
public:
    Oiseau()
    {
        initVar();
        // créer l'oiseau graphique
        initialiserGraphique();

        // créer quelques autres formes
        cube = new FormeCube( 1.0, true );
        quad = new FormeQuad( 1.0, true );
        sphere = new FormeSphere( 1.0, 8, 8, true );
        cylindre = new FormeCylindre( 1.0, 1.0, 1.0, 16, 1, true );
    }
    ~Oiseau()
    {
        conclureGraphique();
        delete cube;
        delete quad;
        delete sphere;
        delete cylindre;
    }

    void initVar() { position = glm::vec3(0.0, 0.0, 2.0); taille = 0.5; angleTete = angleAile = angleBras = 0.0; }
    void verifierAngles() // vérifier que les angles ne débordent pas les valeurs permises
    {
        if ( angleBras > 60.0 ) angleBras = 60.0; else if ( angleBras < 0.0 ) angleBras = 0.0;
        if ( angleAile > 60.0 ) angleAile = 60.0; else if ( angleAile < 0.0 ) angleAile = 0.0;
    }

    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la théière
        glBindVertexArray( vao );

        // (partie 2) MODIFICATIONS ICI ...
        // créer le VBO pour les sommets
        //...
        glGenBuffers(1, &vboTheiereSommets);
        glBindBuffer(GL_ARRAY_BUFFER, vboTheiereSommets);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gTheiereSommets), gTheiereSommets, GL_STATIC_DRAW);
        glVertexAttribPointer(locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(locVertex);

        // créer le VBO la connectivité
        //...
        glGenBuffers(1, &vboTheiereConnec);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTheiereConnec);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gTheiereConnec), gTheiereConnec, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void conclureGraphique()
    {
        glDeleteBuffers( 1, &vboTheiereSommets );
        glDeleteBuffers( 1, &vboTheiereConnec );
    }

    // (partie 2) Vous modifierez cette fonction pour utiliser les VBOs
    // affiche une théière, dont la base est centrée en (0,0,0)
    void afficherTheiere()
    {
        glBindVertexArray( vao );
        // (partie 2) MODIFICATIONS ICI ...
        glDrawElements(GL_TRIANGLES, sizeof(gTheiereConnec)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // vous pouvez utiliser temporairement cette fonction pour la première partie du TP, mais vous ferez mieux dans la seconde partie du TP
        /*glBegin( GL_TRIANGLES );
        for ( unsigned int i = 0 ; i < sizeof(gTheiereConnec)/sizeof(GLuint) ; i++ )
            glVertex3fv( &(gTheiereSommets[3*gTheiereConnec[i]] ) );
        
        glEnd( );*/

        glBindVertexArray(0);
    }

    // afficher l'antenne de l'oiseau
    // L'antenne  est un cylindre de longueur "taille" et de largeur d'un 1/3 de la "taille". Une première partie est fixe au dessus de la tête.
    // Une seconde partie se trouve sur la première partie de l'antenne et tourne deux fois plus vite que angle corps sur elle même selon l'axe Z.
    void afficherAntenne()
    {
        // donner la couleur de l'antenne
        glVertexAttrib3f( locColor, 1.0, 0.5, 1.0 ); // magenta clair

        // afficherRepereCourant( ); // débogage: montrer le repère à la position courante

        // afficher la première partie de l'antenne
        matrModel.PushMatrix(); {

            matrModel.Translate(0.0, 0.0, taille); // Code modifie
            matrModel.Scale(taille / 3.0, taille / 3.0, taille); // Code modifie

            // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCylindre();

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        // afficher la seconde partie de l'antenne
        // Procéder comme avec la première partie de l'antenne.
        //...
        matrModel.PushMatrix(); {
            matrModel.Translate(0.0, 0.0, taille * 2); // Code modifie
            matrModel.Rotate(5 * angleTete, 0.0, 0.0, 1.0); // Code modifie
            matrModel.Scale(taille, taille / 2.0, taille / 2.0); // Code modifie

            // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // afficher la tête de l'oiseau
    // La tête de l'oiseau est une sphère ou theière verte de taille "taille" (ou 0.45 * taille pour la théière)
    // Elle possède des yeux jaunes de taille -> 0.4 * taille situé à l'avant de l'oiseau situé sur les cotés de la tête.
    // Les yeux sont à 60 degrés d'écart (30 degrés vers la gauche pour un oeil et 30 degrés vers la droite pour l'autre).
    void afficherTete()
    {
        // donner la couleur de la tête
        glVertexAttrib3f( locColor, 0.0, 1.0, 1.0 ); // cyan

        // afficherRepereCourant( ); // débogage: montrer le repère à la position courante

        // pour la tête
        matrModel.PushMatrix(); {

            //Afficher la tête
            matrModel.PushMatrix(); {
                // afficher le bon modèle
                switch (Etat::modele)
                {
                default:
                case 1: // une sphère
                    matrModel.Scale(taille, taille, taille);
                    glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                    afficherSphere();
                    break;

                case 2: // la théière
                    matrModel.Translate(0.0, 0.0, -taille);
                    matrModel.Rotate(90, 1.0, 0.0, 0.0);
                    matrModel.Scale( 0.45 * taille, 0.45 * taille, 0.45 * taille);
                    glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                    afficherTheiere();
                    break;
                }
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

            // donner la couleur des yeux
            glVertexAttrib3f( locColor, 1.0, 1.0, 0.0 ); // jaune

            // afficher les yeux
            matrModel.PushMatrix(); {
                glVertexAttrib3f(locColor, 1.0, 1.0, 0.0); // jaune
                GLfloat tailleYeux = 0.4 * taille;
                matrModel.PushMatrix(); { // oeil droit
                    matrModel.Rotate(30.0, 0.0, 0.0, 1.0);
                    matrModel.Translate(taille, 0.0, 0.0);
                    matrModel.Scale(tailleYeux, tailleYeux , tailleYeux);

                    // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
                    glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                    afficherSphere();
                }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

                    
                matrModel.PushMatrix(); { // oeil gauche
                    matrModel.Rotate(30.0, 0.0, 0.0, -1.0);
                    matrModel.Translate(taille, 0.0, 0.0);
                    matrModel.Scale(tailleYeux, tailleYeux, tailleYeux);

                    // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
                    glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                    afficherSphere();
                }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // afficher les deux ailes
    void afficherAiles()
    {
        // donner la couleur des ailes
        glVertexAttrib3f( locColor, 0.5, 0.5, 1.0 ); // violet
        GLfloat tailleAile = 2 * taille;
        // ajouter une ou des transformations afin de tracer des *ailes carrées*, de la même largeur que le corps


        matrModel.PushMatrix();{ // Aile Gauche
            matrModel.Translate(-taille, taille, 0.0);
            matrModel.Rotate(angleAile, 1.0, 0.0, 0.0);
            
            matrModel.Scale(tailleAile, tailleAile, 0.0);

            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherQuad();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        matrModel.PushMatrix(); { // Aile Droite
            //TODO: AJOUTER COMMENTAIRE EXPLICATIF!!!!!!
            matrModel.Rotate(180, 0.0, 0.0, 1.0);


            matrModel.Translate(-taille, taille, 0.0);
            matrModel.Rotate(angleAile, 1.0, 0.0, 0.0);
            
            matrModel.Scale(tailleAile, tailleAile, 0.0);

            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherQuad();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // afficher les bras
    // les bras sont des cubes de largeur "largMembre" et longueur "longMembre"
    // Les bras sont situés sur chaque coté de la tête, tournés 70 degrés selon l'axe des X.
    // Ils ont un mouvement de rotation autour de la tête (axe Z) de angleBras du coté vers l'arrière du robot
    void afficherBras()
    {
        // donner la couleur des bras
        glVertexAttrib3f( locColor, 0.0, 1.0, 0.0 ); // vert

        // ajouter une ou des transformations afin de tracer les bras de largeur "largMembre" et longueur "longMembre"
        // les bras
        matrModel.PushMatrix();{ // Bras 1
            matrModel.Rotate(angleBras, 0.0, 0.0, 1.0);
            matrModel.Translate(0.0, taille + longMembre / 2, -0.1);
            matrModel.Rotate(- 90 + 70, 1.0, 0.0, 0.0);
            matrModel.Scale(largMembre, longMembre, largMembre);
            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        matrModel.PushMatrix(); { // Bras 2
            matrModel.Rotate(angleBras, 0.0, 0.0, -1.0);
            matrModel.Translate(0.0, -taille - longMembre / 2, -0.1);
            matrModel.Rotate(- 90 + 70, -1.0, 0.0, 0.0);
            matrModel.Scale(largMembre, longMembre, largMembre);
            
            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // afficher les jambes
    // les jambes sont des cubes de largeur "largMembre" et longueur "longMembre"
    // Les jambes sont figés en bas de la tête du robot à 60 degrés d'écart (30 degrés vers la gauche pour une jambe et 30 degrés vers la droite pour l'autre).
    void afficherJambes()
    {
        // donner la couleur des jambes
        glVertexAttrib3f( locColor, 0.9, 0.4, 0.0 ); // marron;

        // ajouter une ou des transformations afin de tracer les jambes de largeur "largMembre" et longueur "longMembre"
        // les jambes
        // ...
        matrModel.PushMatrix(); { // Jambe 1
            matrModel.Rotate(90-30, -1.0, 0.0, 0.0);
            matrModel.Translate(0.0, taille + longMembre / 2, 0.0);
            
            
            matrModel.Scale(largMembre, longMembre, largMembre);
            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        matrModel.PushMatrix(); { // Jambe 2
            matrModel.Rotate(90-30, 1.0, 0.0, 0.0);
            matrModel.Translate(0.0, -taille - longMembre / 2, 0.0);
            

            matrModel.Scale(largMembre, longMembre, largMembre);

            // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    void afficher()
    {
        // afficherRepereCourant( ); // débogage: montrer le repère à la position courante
        matrModel.PushMatrix();{ // sauvegarder la tranformation courante

            // ajouter une ou des transformations afin de centrer le haut du corps à la position courante "position[]" et de tourner son corps de l'angle "angleTete"
            //...
            matrModel.Translate(position.x, position.y, position.z);
            matrModel.Rotate(angleTete, 0.0, 0.0, 1.0);

            // afficher l'antenne
            afficherAntenne();

            // afficher la tête
            afficherTete();

            // afficher les deux ailes
            afficherAiles();

            // afficher les deux bras
            afficherBras();

            // afficher les deux jambes
            afficherJambes();

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void calculerPhysique()
    {
        if ( Etat::enmouvement )
        {
            static int sens[6] = { +1, +1, +1, +1, +1, +1 };
            glm::vec3 vitesse( 0.03, 0.02, 0.05 );
            // mouvement en X
            if ( position.x-taille <= -0.5*Etat::dimBoite ) sens[0] = +1;
            else if ( position.x+taille >= 0.5*Etat::dimBoite ) sens[0] = -1;
            position.x += vitesse.x * sens[0];
            // mouvement en Y
            if ( position.y-taille <= -0.5*Etat::dimBoite ) sens[1] = +1;
            else if ( position.y+taille >= 0.5*Etat::dimBoite ) sens[1] = -1;
            position.y += vitesse.y * sens[1];
            // mouvement en Z
            if ( position.z-taille <= 0.0 ) sens[2] = +1;
            else if ( position.z+taille >= Etat::dimBoite ) sens[2] = -1;
            position.z += vitesse.z * sens[2];

            // angle des jambes et des ailes
            if ( angleBras <= 0.0 ) sens[3] = +1;
            else if ( angleBras >= 60.0 ) sens[3] = -1;
            angleBras += 1.0 * sens[3];
            if ( angleAile <= 0.0 ) sens[4] = +1;
            else if ( angleAile >= 60.0 ) sens[4] = -1;
            angleAile += 2.0 * sens[4];

            // taille du corps
            if ( taille <= 0.25 ) sens[5] = +1;
            else if ( taille >= 1.0 ) sens[5] = -1;
            taille += 0.005 * sens[5];

            // rotation du corps
            if ( angleTete > 360.0 ) angleTete -= 360.0;
            angleTete += 0.35;

        }
    }

    // partie 2: utilisation de vbo et vao
    GLuint vao = 0;
    GLuint vboTheiereSommets = 0;
    GLuint vboTheiereConnec = 0;
    GLint locVertex = -1;
    GLint locColor = -1;

    glm::vec3 position;       // position courante de l'oiseau
    GLfloat taille;           // facteur d'échelle du corps
    GLfloat angleTete;       // angle de rotation (en degrés) de l'oiseau
    GLfloat angleAile;        // angle de rotation (en degrés) des ailes
    GLfloat angleBras;        // angle de rotation (en degrés) des bras
    const GLfloat longMembre = 0.7;  // longueur des membres
    const GLfloat largMembre = 0.1;  // largeur des membres
};

#endif
