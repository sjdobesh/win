//================//
//                //
//    shader.h    //
//                //
//========================//
// manage opengl textures //
//============================================================================80

#ifndef _SHADER_H_
#define _SHADER_H_

// SDL & opengl
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

typedef enum shader_type {
  FRAG, VERT, GEO
} shader_type;

typedef struct shader {
  GLuint gl_ptr;
  union { char* name, *path; };
  shader_type type;
} shader;

// struct functions
shader new_shader(char* name, unsigned int type);
void print_shader(shader s);

// shader functions
char* load_shader_code(char* path);
void compile_shader(shader s, const char* source);

#endif
