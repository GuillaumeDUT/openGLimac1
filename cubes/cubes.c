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

#define ZOOM 500
#define DIVIDETIME 50

#define WINDOW_SCALE 2

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
    gluOrtho2D(-WINDOW_SCALE/2.0, WINDOW_SCALE/2., -WINDOW_SCALE/2., WINDOW_SCALE/2.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void drawCube(int full){
  glBegin(GL_QUADS);

    glColor3f(1.0f,0.0f,0.0f);
      glVertex3f(-0.5f,+0.5f,0.5f);
      glVertex3f(0.5f,+0.5f,0.5f);
      glVertex3f(0.5f,-0.5f,0.5f);
      glVertex3f(-0.5f,-0.5f,0.5f);

    glColor3f(0.0f,1.0f,0.0f);
      glVertex3f(-0.5f,0.5f,-0.5f);
      glVertex3f(0.5f,0.5f,-0.5f);
      glVertex3f(0.5f,-0.5f,-0.5f);
      glVertex3f(-0.5f,-0.5f,-0.5f);

    glColor3f(0.0f,0.0f,1.0f);
      glVertex3f(-0.5f,0.5f,0.5f);
      glVertex3f(-0.5f,0.5f,-0.5f);
      glVertex3f(-0.5f,-0.5f,-0.5f);
      glVertex3f(-0.5f,-0.5f,0.5f);

    glColor3f(1.0f,1.0f,0.0f);
      glVertex3f(0.5f,0.5f,0.5f);
      glVertex3f(0.5f,0.5f,-0.5f);
      glVertex3f(0.5f,-0.5f,-0.5f);
      glVertex3f(0.5f,-0.5f,0.5f);

    glColor3f(1.0f,0.0f,1.0f);
      glVertex3f(-0.5f,0.5f,0.5f);
      glVertex3f(-0.5f,0.5f,-0.5f);
      glVertex3f(0.5f,0.5f,-0.5f);
      glVertex3f(0.5f,0.5f,0.5f);

    glColor3f(0.0f,1.0f,1.0f);
      glVertex3f(-0.5f,-0.5f,0.5f);
      glVertex3f(-0.5f,-0.5f,-0.5f);
      glVertex3f(0.5f,-0.5f,-0.5f);
      glVertex3f(0.5f,-0.5f,0.5f);
  glEnd();

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
    SDL_WM_SetCaption("Mdr le s o l e i l", NULL);

    resizeViewport();

    float rotate = 1;
    int trigger =0;

    //printf("%f \n",systemeSolaire[0]->size);

    //SDL_FreeSurface(SDL_Surface* logoImac);

    /* Boucle de dessin (à décommenter pour l'exercice 3)*/
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);

        // glPushMatrix();
        //   glRotatef(rotate,0,0,1);
        //   drawSquare(0);
        // glPopMatrix();
        // rotate+=1;

        drawCube(1);
        glRotatef(rotate,1,0,1);
        rotate+=5;

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
                    break;
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
