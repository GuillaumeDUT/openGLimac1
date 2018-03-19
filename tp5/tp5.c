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

#define WINDOW_SCALE 40

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

typedef struct planet {
  char name[100];
  float size; // Given in AU
  float sun_dist; // Given in AU
  float rot_speed; // Given in km/s
  float angle;
  int r, v, b;
} *Planet;



Planet ajouterPlanete(char name[100], float size, float sun_dist, float rot_speed, int r, int v, int b){
  Planet temp = malloc(sizeof(*temp));
  if(temp == NULL){
    printf("Erreur malloc\n");
    exit(0);
  }
  temp->size = size;

  strcpy(temp->name, name);
  temp->sun_dist = sun_dist;
  temp->rot_speed = rot_speed;
  temp->r = r;
  temp->v = v;
  temp->b = b;
  temp->angle=0;

  return temp;
}

void afficherPlanet(Planet planet){
  //printf("%s\n",planet->name);

  glColor3ub(255,255,255);


  glColor3ub(planet->r,planet->v,planet->b);
  glPushMatrix();
    glRotatef(planet->angle,0,0,1);
    glTranslatef(planet->sun_dist,0,0);
    glScalef(planet->size,planet->size,1);
    glScalef(ZOOM,ZOOM,1);
    drawCircle(1);
  glPopMatrix();
  planet->angle+=planet->rot_speed/DIVIDETIME;
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


    Planet systemeSolaire[100];

    for(int i=0;i<100;i++){
      systemeSolaire[i]=0;
    }

    systemeSolaire[0] = ajouterPlanete("sun", 0.0047, 0, 0, 255, 204, 0);
    systemeSolaire[1] = ajouterPlanete("mercury", 0.00001626, 0.466, 47, 51, 51, 51); //
    systemeSolaire[2] = ajouterPlanete("venus", 0.00004034, 0.728, 35, 155, 155, 155);
    systemeSolaire[3] = ajouterPlanete("earth", 0.00004247, 1, 30, 51, 153, 255);
    systemeSolaire[4] = ajouterPlanete("mars", 0.000022593, 1.666, 24, 255, 153, 51);
    systemeSolaire[5] = ajouterPlanete("jupiter", 0.000466667, 5.458, 13, 255, 204, 102);
    systemeSolaire[6] = ajouterPlanete("saturn", 0.0004, 10.123, 9, 255, 51, 204);
    systemeSolaire[7] = ajouterPlanete("uranus", 0.000166, 20.11, 6.8, 102, 204, 255);
    systemeSolaire[8] = ajouterPlanete("neptune", 0.00016, 30.33, 5.43, 10, 255, 10);

    //printf("%f \n",systemeSolaire[0]->size);

    //SDL_FreeSurface(SDL_Surface* logoImac);

    /* Boucle de dessin (à décommenter pour l'exercice 3)*/
    int loop = 1;
    glClearColor(0.1, 0.1, 0.1 ,1.0);
    while(loop) {

        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);


        // Fin du code de dessin
        for(int i=0;i<9;i++){
          afficherPlanet(systemeSolaire[i]);
        }
        //dessin du contour noir
        /*  HORLOGE
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
        */

        /* SYSTEME SOLAIRE */

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
