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
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// prototypes
void color_red() ;
void color_green() ;
void color_blue() ;
void screentonormalized(float* pos_x, float* pos_y, float* dim_x, float* dim_y, int w, int h) ;
void debug_draw(int* vertex_array, int length) ;

void bind_vao(GLuint vao);
void bind_vbo(GLuint vbo, const float *vertex_array, int length);
void bind_ebo(GLuint ebo, const int *indicies, int length);
void set_vertex_attributes(GLuint program_ptr);
void init_geometry(
    GLuint program_ptr, GLuint vbo, GLuint ebo,
    float* verts, int vert_count,
    int* indicies, int idx_count
);

#endif
