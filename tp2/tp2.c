#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define SEGMENTSCERCLES 64

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 600;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


void resizeViewport() {
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-4., 4., -3., 3.);

    }

void drawSquare(int full){
  GLenum type = full ? GL_QUADS : GL_LINE_LOOP;
  glBegin(type);
    glVertex2f(0.5,0.5);
    glVertex2f(0.5,-0.5);
    glVertex2f(-0.5,-0.5);
    glVertex2f(-0.5,0.5);
  glEnd();
}

void drawLandMark(){
  glColor3ub(255,0,0);
  glBegin(GL_LINES);
    glVertex2f(-4,0);
    glVertex2f(4,0);
  glEnd();

  for(int i=-4;i<4;i++){
    glBegin(GL_LINES);
      glVertex2f(i,0.05);
      glVertex2f(i,-0.05);
    glEnd();
  }

  glColor3ub(0,255,0);
  glBegin(GL_LINES);
    glVertex2f(0,-3);
    glVertex2f(0,3);
  glEnd();
  for(int i=-3;i<3;i++){
    glBegin(GL_LINES);
      glVertex2f(0.05,i);
      glVertex2f(-0.05,i);
    glEnd();
  }

}

void drawCircle(int full){
  GLenum type = full ? GL_POLYGON : GL_LINE_LOOP;
  glBegin(type);
    for(int i=0;i<SEGMENTSCERCLES;i++){
      glVertex2f(cos(i*(2*M_PI/SEGMENTSCERCLES))/2,sin(i*(2*M_PI/SEGMENTSCERCLES))/2);
    }
  glEnd();
}

int main(int argc, char** argv) {

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("J'VEUX DU L J'VEUX DU V j'VEUX DU G", NULL);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


    //glScalef(0.25,0.25,1);
    resizeViewport();
    /* On créé une première primitive par défaut */
    int loop=1;
    float posX = 0;
    float posY = 0;
    float angle =0 ;
    int rightClic = 0;

    float posCercleX =0;
    float posCercleY =0;
    float randomX =0 ;
    float randomY =0 ;
    /* Boucle d'affichage */
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Code de dessin */

        glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer

        drawSquare(0);
        drawLandMark();

        glTranslatef(1,2,0);
        glColor3ub(255, 127, 0);
        drawCircle(1);
        glTranslatef(-1,-2,0);

        glTranslatef(2,0,0);
        glRotatef(45,0,0,1);
        glColor3ub(255, 127, 0);
        drawSquare(1);
        glRotatef(-45,0,0,1);
        glTranslatef(-2,0,0);

        glRotatef(45,0,0,1);
        glTranslatef(2,0,0);
        glColor3ub(128, 0, 128);
        drawSquare(1);
        glTranslatef(-2,0,0);
        glRotatef(-45,0,0,1);




        glTranslatef(posX,posY,0);
        glRotatef(angle,0,0,1);
        glColor3ub(255,255,0);
        drawSquare(1);
        glRotatef(-angle,0,0,1);
        glTranslatef(-posX,-posY,0);

        if(posCercleX>4)
          posCercleX=4;
        if(posCercleX<-4)
          posCercleX=-4;
        if(posCercleY>3)
          posCercleY=3;
        if(posCercleY<-3)
          posCercleY=-3;


        posCercleX = rand()%2  ? posCercleX +0.05 : posCercleX-0.05;
        posCercleY = rand()%2  ? posCercleY + 0.05: posCercleY-0.05;
        glTranslatef(posCercleX,posCercleY,0);
        glColor3ub(0,0,255);
        drawCircle(1);
        glTranslatef(-posCercleX,-posCercleY,0);


        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
              switch (e.type) {


                case SDL_MOUSEBUTTONDOWN:

                    if(e.button.button == SDL_BUTTON_RIGHT){
                      rightClic=1;
                    }else{
                      posX = -4 +8. * e.button.x/WINDOW_WIDTH;
                      posY = -(-3 +6. *  e.button.y/WINDOW_HEIGHT);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    rightClic=0;
                    break;
                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();
                case SDL_MOUSEMOTION:
                    if(rightClic)
                      angle = 360*e.button.x/WINDOW_WIDTH;
                    break;
                default:
                    break;
            }

        }

        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
