//================//
//                //
//    shader.c    //
//                //
//========================//
// manage opengl textures //
//============================================================================80

// struct functions
// 28  - shader new_shader(char* name, unsigned int type);
// 49  - program new_program(char* name);
// 55  - void print_shader(shader s);
// 63  - void print_program(program p);

// shader functions
// 72  - void use_program(program p);
// 78  - void unuse_program(program p);
// 84  - char* load_shader_code(char* path);
// 117 - void compile_shader(shader s, const char* source);
// 136 - GLuint link_program(GLuint program, GLuint vert, GLuint frag);
// 145 - program load_new_program(char* name, char* vert_path, char* frag_path);

#include <stdlib.h>
#include <string.h>
#include "shader.h"

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

// print out program struct
void print_shader(shader s) {
  printf("shader [\n");
  printf("  gl ptr : %d\n", s.gl_ptr);
  printf("]\n");
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

