//================//
//                //
//    SHADER.H    //
//                //
//========================//
// manage opengl textures //
//============================================================================80

// SDL & OpenGL
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

enum shader_type {
  FRAG, VERT, GEO
};

typedef struct shader {
  GLuint gl_ptr;        // opengl reference
  char* name;           // user given name
  unsigned int type;    // shader type enum
} shader;

typedef struct program {
  GLuint gl_ptr;
  GLuint vao, ebo, vbo; // shader geometry data
  shader* vert, * frag; // ptrs to the shaders this program is using
  char* name;           // user given name
  int bound;            // bool to check if in use
} program;


// PROTOTYPES

// shader functions
char* load_shader_code(char* path);
void compile_shader(shader s, const char* source);
GLuint link_program(GLuint program, GLuint vert, GLuint frag);
shader new_shader(char* name, unsigned int type);
program new_program(char* name);
program load_new_program(char* name, char* vert_path, char* frag_path);
void use_program(program* p);
void unuse_program(program* p);
