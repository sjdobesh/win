//================//
//                //
//    shader.c    //
//                //
//=====================================//
// manage opengl shaders and programs. //
//============================================================================80

// TOC

// struct functions
// 32  - new_shader
// 53  - new_program
// 59  - print_shader
// 67  - print_program

// shader functions
// 76  - use_program
// 82  - unuse_program
// 88  - load_shader_code
// 121 - compile_shader
// 140 - link_program
// 149 - load_new_program
// 179 - update_program

#include <stdlib.h>
#include <string.h>
#include "shader.h"
#include "uniform.h"

// return a newly populated shader struct
shader new_shader(char* name, unsigned int type) {
  GLuint gl_ptr;
  switch (type) {
    case VERT:
      gl_ptr = glCreateShader(GL_VERTEX_SHADER);
      break;
    case FRAG:
      gl_ptr = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    case GEO:
      fprintf(stderr, "[ERROR] geometry shader unimplemented\n");
      break;
    default:
      fprintf(stderr, "[ERROR] unknown shader type\n");
      break;
  }
  shader s = {.gl_ptr = gl_ptr, .name = name, .type = type};
  return s;
}

// return a newly populated program struct
program new_program(char* name) {
  program p = {.gl_ptr = glCreateProgram(), .name = name};
  return p;
}

// print out program struct
void print_shader(shader s) {
  printf("shader [\n");
  printf("  name : %s\n", s.name);
  printf("  gl ptr : %d\n", s.gl_ptr);
  printf("]\n");
}

// print out program struct
void print_program(program p) {
  printf("program [\n");
  printf("  name : %s\n", p.name);
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

// loads shader code from a file given a path
char* load_shader_code(char* path) {
  FILE *f;
  long size;
  char *buf;
  // try to open it
  if ((f = fopen(path, "r")) == NULL) {
    fprintf(stderr, "[ERROR] couldn't open shader file \n");
    exit(EXIT_FAILURE);
  }
  // check file size
  fseek(f , 0L , SEEK_END);
  size = ftell(f);
  rewind(f);
  // allocate memory
  buf = calloc(1, size + 1);
  if (!buf) {
    fclose(f);
    fprintf(stderr, "[ERROR] memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  // copy to buf
  if(1 != fread(buf , size, 1 , f)) {
    fclose(f);
    free(buf);
    fprintf(stderr, "[ERROR] shader file read failed\n");
    exit(EXIT_FAILURE);
  }
  // close stream
  fclose(f);
  return buf;
}

// compile a vert or frag shader
void compile_shader(shader s, const char* source) {
  GLint status;
  char err_buf[1024];
  glShaderSource(s.gl_ptr, 1, &source, NULL);
  glCompileShader(s.gl_ptr);
  glGetShaderiv(s.gl_ptr, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderInfoLog(s.gl_ptr, sizeof(err_buf), NULL, err_buf);
    err_buf[sizeof(err_buf)-1] = '\0';
    fprintf(
      stderr,
      "[ERROR] shader compilation failed: %s\n",
      err_buf
    );
    exit(EXIT_FAILURE);
  }
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

// update a program struct and return it
program update_program(program p) {
  p.uniforms = update_uniforms(p.uniforms);
  return p;
}
