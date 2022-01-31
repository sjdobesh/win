#include <stdlib.h>
#include <string.h>
#include "shader.h"

// SHADER FUNCTIONS //========================================================80

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
      "[ERROR] %s shader compilation failed\n",
      err_buf
    );
    exit(EXIT_FAILURE);
  }
}

// compile shaders into a usable program
GLuint link_program(GLuint program, GLuint vert, GLuint frag) {
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glBindFragDataLocation(program, 0, "out_Color");
  glLinkProgram(program);
  return program;
}

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

program new_program(char* name) {
  program p = {.gl_ptr = glCreateProgram(), .name = name};
  return p;
}

program load_new_program(char* name, char* vert_path, char* frag_path) {
  shader v = new_shader("vert", VERT);
  shader f = new_shader("frag", FRAG);
  const char* v_src = load_shader_code(vert_path);
  const char* f_src = load_shader_code(frag_path);
  compile_shader(v, v_src);
  compile_shader(f, f_src);
  program p = new_program(name);
  link_program(p.gl_ptr, v.gl_ptr, f.gl_ptr);
  // add reference to frag and vert
  p.vert = &v;
  p.frag = &f;
  return p;
}

void use_program(program* p) {
  glUseProgram(p->gl_ptr);
  p->bound = 1;
}

void unuse_program(program* p) {
  glUseProgram(0);
  p->bound = 0;
}

