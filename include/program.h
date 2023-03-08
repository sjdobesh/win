/*=================*
 *                 *
 *    program.h    *
 *                 *
 *=====================================*
 * samantha jane                       *
 * manage opengl shaders and programs. *
 *==========================================================================80*/

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "shader.h"

/* compiled shader program */
typedef struct program {
  GLuint gl_ptr;         /* points to compiled program */
  GLuint vao, ebo, vbo;  /* shader geometry data */
  shader vert, frag;     /* ptrs to the shaders this program is using */
  unsigned pos_x, pos_y; /* position of top left */
  unsigned dim_x, dim_y; /* width to render */
  uniform uniforms[10];  /* max of 10 uniforms per program */
} program;

GLuint link_program(GLuint program, GLuint vert, GLuint frag);
program update_program(program p);
