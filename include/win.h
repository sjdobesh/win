//=============//
//             //
//    win.h    //
//             //
//==========================================//
// header for SDL window and opengl context //
//============================================================================80

#ifndef _WIN_H_
#define _WIN_H_

// SDL & opengl
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "shader.h"

// structs //-------------------------------------------------------------------

typedef struct win {
  SDL_Window*   window;
  SDL_GLContext context;
  program       prog;
  int w, h;
} win;

// prototypes //----------------------------------------------------------------

win init_win(int h, int w) ;
void render_win(win w) ;
void clean_win(win* w) ;
#endif
