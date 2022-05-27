//============//
//            //
//    gl.h    //
//            //
//===========================//
// header for opengl context //
//============================================================================80

#ifndef _GL_H_
#define _GL_H_

// SDL & opengl
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// custom
#include "win.h"

// prototypes
void color_red();
void color_green();
void color_blue();
void screentonormalized(float *pos_x, float *pos_y, float *dim_x, float *dim_y,
                        int w, int h);
void debug_draw(int *vertex_array, int length);

void bind_vao(win *w);
void bind_vbo(win *w, const float *vertex_array, int length);
void bind_ebo(win *w, const int *indicies, int length);
void bind_vertex_attributes(win *w);
void init_geometry(win *w);

void load_program(win *w, char *vert_path, char *frag_path, int pos_x,
                  int pos_y, int dim_x, int dim_y);
void print_win(win w);
void new_sprite(win *w, char *texture_path, int pos_x, int pos_y, int dim_x,
                int dim_y);

void welcome();
void pause();
void texture_test();
void debug_test();

#endif
