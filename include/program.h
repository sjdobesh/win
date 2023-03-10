//=================//
//                 //
//    program.h    //
//                 //
//========================//
// manage opengl textures //
//============================================================================80

#include "shader.h"
#include "texture.h"

#ifndef _PROGRAM_H_
#define _PROGRAM_H_

typedef struct program {
  GLuint gl_ptr;
  GLuint vao, ebo, vbo;    // shader geometry data
  shader vert, frag;       // ptrs to the shaders this program is using
  texture tex;             // texture ptr for shader uniforms
  union { int x, pos_x; }; // position of top left
  union { int y, pos_y; };
  union { int w, width, dim_x; };
  union { int h, height, dim_y; };
  unsigned bound : 1;               // bool to check if in use
} program;

program new_program(char* name);
void print_program(program p);
void use_program(program* p);
void unuse_program(program* p);
GLuint link_program(GLuint program, GLuint vert, GLuint frag);
program load_new_program(
  char* name,
  char* vert_path, char* frag_path,
  int pos_x, int pos_y,
  int dim_x, int dim_y
);
#endif
