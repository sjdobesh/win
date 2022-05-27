//================//
//                //
//    shader.h    //
//                //
//====================================================//
// opengl shader data including program and uniforms. //
//============================================================================80

// SDL & opengl
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "texture.h"
#include "uniformbuf.h"

enum shader_type { FRAG, VERT, GEO };

typedef struct Shader {
  GLuint gl_ptr;     // opengl reference
  char *name;        // user given name
  unsigned int type; // shader type enum
} Shader;

typedef struct Program {
  GLuint gl_ptr;
  char *name;           // user given name
  GLuint vao, ebo, vbo; // shader geometry data
  Shader vert, frag;    // ptrs to the shaders this program is using
  Texture tex;          // texture ptr for shader uniforms
  int pos_x, pos_y;     // position of top left
  int dim_x, dim_y;     // width to render
  int bound;            // bool to check if this program is in use
  UniformBuf ubuf;      // uniform data and update functions
} Program;

// struct functions
Shader new_shader(char *name, unsigned int type);
Program new_program(char *name);
void print_shader(Shader s);
void print_program(Program p);

// shader functions
void use_program(Program *p);
void unuse_program(Program *p);
char *load_shader_code(char *path);
void compile_shader(Shader s, const char *source);
GLuint link_program(GLuint program, GLuint vert, GLuint frag);
Program load_new_program(char *name, char *vert_path, char *frag_path,
                         int pos_x, int pos_y, int dim_x, int dim_y);
