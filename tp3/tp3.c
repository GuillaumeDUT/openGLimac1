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
    gluOrtho2D(-8., 8., -6., 6.);

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
    glVertex2f(-8,0);
    glVertex2f(8,0);
  glEnd();

  for(int i=-8;i<8;i++){
    glBegin(GL_LINES);
      glVertex2f(i,0.05);
      glVertex2f(i,-0.05);
    glEnd();
  }

  glColor3ub(0,255,0);
  glBegin(GL_LINES);
    glVertex2f(0,-6);
    glVertex2f(0,6);
  glEnd();
  for(int i=-6;i<6;i++){
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

void drawRoundedSquare(float taille){
  glPushMatrix();
    glScalef(taille,1,1);
    drawSquare(1);
  glPopMatrix();
  glPushMatrix();
    glScalef(1,taille,1);
    drawSquare(1);
  glPopMatrix();
  //cercle haut droit
  glPushMatrix();
    glTranslatef(taille/2,taille/2,0.0);
    glPushMatrix();
      glScalef(taille,taille,1);
      drawCircle(1);
    glPopMatrix();
  glPopMatrix();
  //cercle haut gauche
  glPushMatrix();
    glTranslatef(-taille/2,taille/2,0.0);
    glPushMatrix();
      glScalef(taille,taille,1);
      drawCircle(1);
    glPopMatrix();
  glPopMatrix();
  //cercle bas droit
  glPushMatrix();
    glTranslatef(taille/2,-taille/2,0.0);
    glPushMatrix();
      glScalef(taille,taille,1);
      drawCircle(1);
    glPopMatrix();
  glPopMatrix();
  //cercle bas gauche
  glPushMatrix();
    glTranslatef(-taille/2,-taille/2,0.0);
    glPushMatrix();
      glScalef(taille,taille,1);
      drawCircle(1);
    glPopMatrix();
  glPopMatrix();
}

void drawFirstArm(){
  glColor3ub(0,255,125);
  glPushMatrix();
    glBegin(GL_POLYGON);
      glVertex2f(3,0.5);
      glVertex2f(3,-0.5);
      glVertex2f(-3,-1);
      glVertex2f(-3,1);
    glEnd();
  glPopMatrix();


  glColor3ub(255,125,0);
  glPushMatrix();
    glTranslatef(-3,0.0,0.0);
    glPushMatrix();
      glScalef(2,2,1);
      drawCircle(1);
    glPopMatrix();
  glPopMatrix();

  glColor3ub(255,0,125);
  glPushMatrix();
    glTranslatef(3,0.0,0.0);
    drawCircle(1);
  glPopMatrix();
}

void drawSecondArm(){
  glColor3ub(255,255,255);
  glPushMatrix();
    glTranslatef(2,0,0);
    drawRoundedSquare(0.5);
  glPopMatrix();
  glPushMatrix();
    glTranslatef(-2,0,0);
    drawRoundedSquare(0.5);
  glPopMatrix();

  glColor3ub(50,50,50);
  glPushMatrix();
    glScalef(2.3,0.3,0.0);
    glBegin(GL_POLYGON);
      glVertex2f(1,1);
      glVertex2f(1,-1);
      glVertex2f(-1,-1);
      glVertex2f(-1,1);
    glEnd();
  glPopMatrix();
}

void drawThirdArm(){
  glColor3ub(0,255,0);
  glPushMatrix();
    glTranslatef(-2,0,0);
    glPushMatrix();
      glScalef(0.6,0.6,1);
      drawRoundedSquare(0.5);
    glPopMatrix();
  glPopMatrix();

  glColor3ub(255,0,0);
  glPushMatrix();
    glScalef(2,0.2,1);
    glBegin(GL_POLYGON);
      glVertex2f(1,1);
      glVertex2f(1,-1);
      glVertex2f(-1,-1);
      glVertex2f(-1,1);
    glEnd();
  glPopMatrix();

  glColor3ub(0,125,255);
  glPushMatrix();
    glTranslatef(2,0,0);
    glPushMatrix();
      glScalef(0.8,0.8,0);
      drawCircle(1);
    glPopMatrix();
  glPopMatrix();
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


    /* Boucle d'affichage */
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Code de dessin */

        glClear(GL_COLOR_BUFFER_BIT); // Toujours commencer par clear le buffer
        drawLandMark();

        //drawRoundedSquare(0.5);

         drawFirstArm();
        //
         drawSecondArm();
         drawThirdArm();
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
                    break;
                case SDL_MOUSEBUTTONUP:

                    break;
                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();
                case SDL_MOUSEMOTION:

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
