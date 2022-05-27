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
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "shader.h"

// structs //-------------------------------------------------------------------

typedef struct Win {
  SDL_Window *window;
  SDL_GLContext context;
  Program program;
  UniformBuf uniformbuf;
  int w, h;
} Win;

// prototypes //----------------------------------------------------------------

Win init_win(int h, int w);
void init_sdl(Win *w);
void init_context(Win *w);
void win_render(Win w);
void win_clean(Win *w);
#endif
