//============//
//            //
//    gl.c    //
//            //
//=======================================================//
// samantha jane                                         //
// handle opengl function calls, settings, and utilities //
//============================================================================80

// sdl & opengl
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "gl.h"

// rendering //-----------------------------------------------------------------

void color_red() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void color_green() {
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void color_blue() {
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

// convert a screen space box to normalized coordinates
void screentonormalized(float *pos_x, float *pos_y, float *dim_x, float *dim_y,
                        int w, int h) {
  /// convert range from 0 to w to -1 to 1
  // 2x/w - 1, -2x/h + 1
  printf("win: %dx%d\n", w, h);
  printf("pos : %.2f, %.2f dim : %.2f, %.2f\n", *pos_x, *pos_y, *dim_x, *dim_y);
  *pos_x = ((2.0 / (float)w) * *pos_x) - 1.0;
  *pos_y = ((-2.0 / (float)h) * *pos_y) + 1.0;
  *dim_x = ((2.0 / (float)w) * *dim_x) - 1.0;
  *dim_y = ((-2.0 / (float)h) * *dim_y) + 1.0;
  printf("pos : %.2f, %.2f dim : %.2f, %.2f\n", *pos_x, *pos_y, *dim_x, *dim_y);
}

// draw an arbitrary set of vertices to the screen
void debug_draw(int *vertex_array, int length) {
  glDrawArrays(GL_LINES, 0, length);
}

// binding buffers //-----------------------------------------------------------

// generate and bind a vertex array object (vao) to the program in the window
void bind_vao(GLuint vao) {
  glGenVertexArrays(1, &(vao));
  glBindVertexArray(vao);
}

// generate and bind a vertex buffer object (vbo) to the program in the window
void bind_vbo(GLuint vbo, const float *vertex_array, int length) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, length * sizeof(GLfloat), vertex_array,
               GL_STATIC_DRAW);
}

// generate and bind an element buffer object (ebo) to the program in the window
void bind_ebo(GLuint ebo, const int *indicies, int length) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(GLint), indicies,
               GL_STATIC_DRAW);
}

// geometry //------------------------------------------------------------------

// set attributes in the vertex shader
void set_vertex_attributes(GLuint program_ptr) {
  // vertex position
  GLint pos_attr_loc = glGetAttribLocation(program_ptr, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(pos_attr_loc);
  // texture coordinate
  GLint tex_attr_loc = glGetAttribLocation(program_ptr, "in_Texcoord");
  glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(tex_attr_loc);
}

// initialize an OpenGL geometry data
void init_geometry(
    GLuint program_ptr, GLuint vbo, GLuint ebo,
    float* verts, int vert_count,
    int* indicies, int idx_count)
{
  glGenBuffers(1, &(vbo));
  glGenBuffers(1, &(ebo));
  bind_vbo(vbo, (const float *)verts, vert_count);
  bind_ebo(ebo, (const int *)indicies, idx_count);
  set_vertex_attributes(program_ptr);
}
