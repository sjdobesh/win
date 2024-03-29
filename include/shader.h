/*================*
 *                *
 *    shader.h    *
 *                *
 *=====================================*
 * manage opengl shaders and programs. *
 *==========================================================================80*/

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

/* vert/frag shader */
typedef struct shader {
  GLuint gl_ptr;
  union { char* name, *path; };
  shader_type type;
} shader;

// struct functions
shader new_shader(char* name, shader_type type);
void shader_print(shader s);
void shader_tab_print(shader s, unsigned tabs);

/* print out a shader struct */
void shader_tab_print(shader s, unsigned tab_level);

// shader functions
char* load_shader_code(char* path);
void compile_shader(shader s, const char* source);

#endif
