/*================*
 *                *
 *    shader.h    *
 *                *
 *=====================================*
 * manage opengl shaders and programs. *
 *==========================================================================80*/

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "uniform.h"

/* fragment, vertex */
enum shader_type {
  FRAG, VERT
};

/* vert/frag shader */
typedef struct shader {
  GLuint gl_ptr;            // opengl reference
  union {
    char* name, path;       // name or path
  };
  shader_type type;         // shader type enum
} shader;

/* struct functions */
char* load_shader_code(char* path);
void compile_shader(shader s, const char* source);
void shader_print(shader s);
void shader_tab_print(shader s, unsigned tab_level);
