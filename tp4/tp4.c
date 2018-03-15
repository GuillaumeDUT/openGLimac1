#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL/SDL_image.h>

static unsigned int WINDOW_WIDTH = 800;
static unsigned int WINDOW_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void resizeViewport() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);
}

const char* filename = "logo_imac_400x400.jpg";
const char* imgPath0 = "numbers/0.png";
const char* imgPath1 = "numbers/1.png";
const char* imgPath2 = "numbers/2png";
const char* imgPath3 = "numbers/3.png";
const char* imgPath4 = "numbers/4.png";
const char* imgPath5 = "numbers/5.png";
const char* imgPath6 = "numbers/6.png";
const char* imgPath7 = "numbers/7png";
const char* imgPath8 = "numbers/8.png";
const char* imgPath9 = "numbers/9.png";
const char* imgPathColon = "numbers/colon.png";

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

    // TODO: Chargement et traitement de la texture

    SDL_Surface *logoImac;
    logoImac = IMG_Load(filename);

    SDL_Surface *img0;
    img0 = IMG_Load(imgPath0);

    SDL_Surface *img1;
    img1 = IMG_Load(imgPath1);

    SDL_Surface *img2;
    img2 = IMG_Load(imgPath2);

    SDL_Surface *img3;
    img3 = IMG_Load(imgPath3);

    SDL_Surface *img4;
    img4 = IMG_Load(imgPath4);

    SDL_Surface *img5;
    img5 = IMG_Load(imgPath5);

    SDL_Surface *img6;
    img6 = IMG_Load(imgPath6);

    SDL_Surface *img7;
    img7 = IMG_Load(imgPath7);

    SDL_Surface *img8;
    img8 = IMG_Load(imgPath8);

    SDL_Surface *img9;
    img9 = IMG_Load(imgPath9);

    SDL_Surface *imgColon;
    imgColon = IMG_Load(imgPathColon);


    GLuint textureID;
    glGenTextures(1,&textureID);

    glBindTexture(GL_TEXTURE_2D,textureID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,logoImac->w,logoImac->h,0,GL_RGB,GL_UNSIGNED_BYTE,logoImac->pixels);
    glBindTexture(GL_TEXTURE_2D,0);

    SDL_FreeSurface(logoImac);



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

        glEnable(GL_TEXTURE_2D); //Activation du texturing;
        glBindTexture(GL_TEXTURE_2D,textureID);

        // ...
        // glPushMatrix();
        //   glScalef(0.8,0.8,0);
        //   glBegin(GL_QUADS);
        //     glTexCoord2f(1,0);
        //     glVertex2f(0.5,0.5);
        //
        //     glTexCoord2f(1,1);
        //     glVertex2f(0.5,-0.5);
        //
        //     glTexCoord2f(0,1);
        //     glVertex2f(-0.5,-0.5);
        //
        //     glTexCoord2f(0,0);
        //     glVertex2f(-0.5,0.5);
        //   glEnd();
        // glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);

        // Fin du code de dessin

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

    glDeleteTextures(1,&textureID);

    // TODO: Libération des données GPU
    // ...

    // Liberation des ressources associées à la SDL
    SDL_Quit();

    return EXIT_SUCCESS;
}
