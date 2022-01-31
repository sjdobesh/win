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


// window parameters for opengl and sdl
// typedef struct win_parameters {
//   SDL_Window*     window;
//   SDL_GLContext   context;
//   GLuint          vao, vbo, ebo, tex;
//   GLuint          vert_shader, frag_shader;
//   GLuint          shader_prog;
//   char*           vert_path;
//   char*           frag_path;
//   int             w, h;
// } win_parameters;
typedef struct win {
  SDL_Window*       window;
  SDL_GLContext     context;
  program*          prog;
  int w, h;
} win;

// // texture parameters to load in an image
// typedef struct tex_parameters {
//   char* path;      // path to this texture
//   int w, h, c;     // width, height, channels
//   char* pixel_buf; // buffer for pixel bytes
// } tex_parameters;

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
