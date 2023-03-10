//=================//
//                 //
//    program.c    //
//                 //
//========================//
// manage opengl textures //
//============================================================================80
#include "program.h"

// return a newly populated program struct
program new_program(char* name) {
  program p = {.gl_ptr = glCreateProgram()};
  return p;
}


// print out program struct
void print_program(program p) {
  printf("program [\n");
  printf("  gl ptr : %d\n", p.gl_ptr);
  printf("  bound : %d\n", p.bound);
  printf("]\n");
}

// set a program to be in use by opengl
void use_program(program* p) {
  glUseProgram(p->gl_ptr);
  p->bound = 1;
}

// unset a program to be in use by opengl
void unuse_program(program* p) {
  glUseProgram(0);
  p->bound = 0;
}

// link shaders into a usable program
GLuint link_program(GLuint program, GLuint vert, GLuint frag) {
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glBindFragDataLocation(program, 0, "out_Color");
  glLinkProgram(program);
  return program;
}

// load a program from paths
program load_new_program(
  char* name,
  char* vert_path, char* frag_path,
  int pos_x, int pos_y,
  int dim_x, int dim_y
) {
  // make new shader structs
  shader v = new_shader("vert", VERT);
  shader f = new_shader("frag", FRAG);
  // load code into memory
  const char* v_src = load_shader_code(vert_path);
  const char* f_src = load_shader_code(frag_path);
  // compile vert and frag
  compile_shader(v, v_src);
  compile_shader(f, f_src);
  // link into new program
  program p = new_program(name);
  link_program(p.gl_ptr, v.gl_ptr, f.gl_ptr);
  // add reference to frag and vert and return
  p.vert = v;
  p.frag = f;
  // set size
  p.pos_x = pos_x;
  p.pos_y = pos_y;
  p.dim_x = dim_x;
  p.dim_y = dim_y;
  return p;
}
