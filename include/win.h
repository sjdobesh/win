//=============//
//             //
//    WIN.H    //
//             //
//==========================================//
// Header for SDL window and openGL context //
//============================================================================80

#ifndef _WIN_H_
#define _WIN_H_

// SDL & OpenGL
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "shader.h"

// STRUCTS //-------------------------------------------------------------------

typedef struct win {
  SDL_Window*       window;
  SDL_GLContext     context;
  program           prog;
  int w, h;
} win;

// PROTOTYPES //----------------------------------------------------------------

// external functions
win init_win(int h, int w);
void win_render(win w);
void win_clean(win* w);
void run_shader(char* vert_path, char* frag_path);
// internal functions
void init_sdl(win* w);
void init_context(win* w);
void init_win_shaders(win* w);

#endif
