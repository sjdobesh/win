/*================*
 *                *
 *    shader.c    *
 *                *
 *=====================================*
 * manage opengl shaders and programs. *
 *==========================================================================80*/

#include <stdlib.h>
#include <string.h>
#include "shader.h"

/* return a newly populated shader struct */
shader new_shader(char* name, unsigned int type) {
  GLuint gl_ptr;
  switch (type) {
    case VERT:
      gl_ptr = glCreateShader(GL_VERTEX_SHADER);
      break;
    case FRAG:
      gl_ptr = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    default:
      fprintf(stderr, "[ERROR] unknown shader type\n");
      break;
  }
  shader s = {.gl_ptr = gl_ptr, .name = name, .type = type};
  return s;
}

/* loads shader code from a file given a path */
char* load_shader_code(char* path) {
  FILE *f;
  long size;
  char *buf;
  /* try to open it */
  if ((f = fopen(path, "r")) == NULL) {
    fprintf(stderr, "[ERROR] couldn't open shader file \n");
    exit(EXIT_FAILURE);
  }
  /* check file size */
  fseek(f , 0L , SEEK_END);
  size = ftell(f);
  rewind(f);
  /* allocate memory */
  buf = calloc(1, size + 1);
  if (!buf) {
    fclose(f);
    fprintf(stderr, "[ERROR] memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  /* copy to buf */
  if(1 != fread(buf , size, 1 , f)) {
    fclose(f);
    free(buf);
    fprintf(stderr, "[ERROR] shader file read failed\n");
    exit(EXIT_FAILURE);
  }
  /* close stream */
  fclose(f);
  return buf;
}

/* compile a vert or frag shader */
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

/* local tab printer */
void print_tabs(unsigned tabs) {
  for (int i = 0; i < tabs; i++)
    printf("  ");
}

/* print out a shader struct */
void shader_print(shader s) {
  shader_tab_print(s, 0);
}

/* print out a shader struct */
void shader_tab_print(shader s, unsigned tab_level) {
  print_tabs(tab_level);
  printf("SHADER [\n");
  print_tabs(tab_level);
  printf("  path : %s\n", s.path);
  print_tabs(tab_level);
  printf("  type : %s\n", s.type == FRAG ? "FRAG" : "VERT" );
  print_tabs(tab_level);
  printf("]\n");
}
