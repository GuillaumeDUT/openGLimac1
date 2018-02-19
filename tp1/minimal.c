#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH =400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resize(int w, int h){
  SDL_SetVideoMode(w, h, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1., 1., -1., 1.);
}

int main(int argc, char** argv) {

    /* Initialisation de la SDL */
    float x,y;
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("L'OpenGL c'est surfait", NULL);

    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Placer ici le code de dessin */



        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }

            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) {

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    if(e.button.x <=200 && e.button.y <=200 ){
                      glClearColor(0, 0, 0, 1.0);
                      glClear(GL_COLOR_BUFFER_BIT);
                      printf("clic carré haut gauche\n");
                    }
                    if(e.button.x >=200 && e.button.y <=200 ){
                      glClearColor(0, 1.0, 0, 1.0);
                      glClear(GL_COLOR_BUFFER_BIT);
                      printf("clic carré bas gauche\n");
                    }
                    if(e.button.x <=200 && e.button.y >=200 ){
                      glClearColor(1.0, 0, 0, 1.0);
                      glClear(GL_COLOR_BUFFER_BIT);
                      printf("clic carré haut droit\n");
                    }
                    if(e.button.x >=200 && e.button.y >=200 ){
                      glClearColor(1.0, 1.0, 0, 1.0);
                      glClear(GL_COLOR_BUFFER_BIT);
                      printf("clic carré bas droit\n");
                    }
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    if(e.key.keysym.sym == 113){
                      printf("touche Q pressée \n");
                      return EXIT_SUCCESS;
                    }
                    break;

                case SDL_MOUSEMOTION:
                  x = e.motion.x;
                  y = e.motion.y;
                  /*printf("coord X : %d\n",e.motion.x); */

                  glClearColor((x/WINDOW_WIDTH),(y/WINDOW_HEIGHT),0,1);
                  glClear(GL_COLOR_BUFFER_BIT);
                  break;
                case SDL_VIDEORESIZE:
                  resize(e.resize.w,e.resize.h);
                  break;
                default:
                    break;
            }
        }

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();
    }

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
