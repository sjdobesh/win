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
void init_sdl(win* w) ;
void init_context(win* w) ;
void screentonormalized(float* pos_x, float* pos_y, float* dim_x, float* dim_y, int w, int h) ;
void bind_vao(win* w) ;
void bind_vbo(win* w, const float* vertex_array, int length) ;
void bind_ebo(win* w, const int* indicies, int length) ;
void bind_vertex_attributes(win* w) ;
void init_geometry(win* w) ;
void win_clean(win* w) ;
void color_red() ;
void color_green() ;
void color_blue() ;
void win_render(win w) ;
void debug_draw(int* vertex_array, int length) ;
void load_program (
  win* w,
  char* vert_path, char* frag_path,
  int pos_x, int pos_y,
  int dim_x, int dim_y
);
void print_win(win w) ;
void new_sprite (
  win* w,
  char* texture_path,
  int pos_x, int pos_y,
  int dim_x, int dim_y
);
void welcome() ;
void pause() ;
void texture_test();
void debug_test();
#endif
