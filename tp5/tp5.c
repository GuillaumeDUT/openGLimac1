#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL_image.h>
#define SEGMENTSCERCLES 64

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

time_t rawtime;

struct tm * timeinfo;

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}


void drawCircle(int full){
  GLenum type = full ? GL_POLYGON : GL_LINE_LOOP;
  glBegin(type);
    for(int i=0;i<SEGMENTSCERCLES;i++){
      glVertex2f(cos(i*(2*M_PI/SEGMENTSCERCLES))/2,sin(i*(2*M_PI/SEGMENTSCERCLES))/2);
    }
  glEnd();
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

void drawHours(int angle){
  glPushMatrix();
    glRotatef(-30*angle,0,0,1);
    glPushMatrix();
      glTranslatef(0,0.3,0);
      glPushMatrix();
        glScalef(0.05,1.1,0);
        glColor3ub(200,100,0);
        drawSquare(1);
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

void drawMinutes(int angle){
  glPushMatrix();
    glRotatef(-6*angle,0,0,1);
    glPushMatrix();
      glTranslatef(0,0.3,0);
      glPushMatrix();
        glScalef(0.02,1.1,0);
        glColor3ub(0,200,100);
        drawSquare(1);
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

void drawSeconds(int angle){
  glPushMatrix();
    glRotatef(-6*angle,0,0,1);
    glPushMatrix();
      glTranslatef(0,0.3,0);
      glPushMatrix();
        glScalef(0.01,1.1,0);
        glColor3ub(100,0,200);
        drawSquare(1);
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

int main(int argc, char** argv) {

    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    // Ouverture d'une fenêtre et création d'un contexte OpenGL
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("td04", NULL);
    resizeViewport();

    // ...

    // TODO: Libération des données CPU
    // ...

    //SDL_FreeSurface(SDL_Surface* logoImac);

    /* Boucle de dessin (à décommenter pour l'exercice 3)*/
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();

        // TODO: Code de dessin

        glClear(GL_COLOR_BUFFER_BIT);

        // Fin du code de dessin

        //dessin du contour noir

        glPushMatrix();
          glScalef(2,2,1);
          glColor3ub(0,0,0);
          drawCircle(1);
        glPopMatrix();
        //inner Circle
        glPushMatrix();
          glScalef(1.95,1.95,0);
          glColor3ub(222,222,222);
          drawCircle(1);
        glPopMatrix();

        // dessin des "ticks" toutes les 5 minutes
        for(int i=0;i<60;i++){
          glPushMatrix();
            glRotatef(-6*i+6,0,0,1);
            glTranslatef(0,0.85,0);
            glPushMatrix();
              glPushMatrix();
                i%5 == 1 ? glScalef(0.02,0.08,0) : glScalef(0.01,0.05,0);

                glColor3ub(0,0,0);
                drawSquare(1);
              glPopMatrix();
            glPopMatrix();
          glPopMatrix();
        }

        time (&rawtime);
        timeinfo = localtime (&rawtime);

        drawHours(timeinfo->tm_hour);
        drawMinutes(timeinfo->tm_min);
        drawSeconds(timeinfo->tm_sec);

        //printf("hours %d\n", timeinfo->tm_hour);
        //printf("minutes %d\n", timeinfo->tm_min);
        //printf("seconds %d\n", timeinfo->tm_sec);

        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            switch(e.type) {

                case SDL_QUIT:
                    loop = 0;
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    resizeViewport();

                default:
                    break;
            }
        }

        SDL_GL_SwapBuffers();
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }


    // TODO: Libération des données GPU
    // ...

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
