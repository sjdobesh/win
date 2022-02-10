//=================//
//                 //
//    texture.c    //
//                 //
//======================================//
// texture managing component to handle //
// loading and organizing gl textures.  //
//============================================================================80

// SDL & opengl
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// stb image library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// std
#include <string.h>

// custom
#include "texture.h"

// load & bind //---------------------------------------------------------------

// load an image with stbi
char* create_image(char* path, int* w, int* h, int* c) {
  // automatically extracts size and channels
  return (char*)stbi_load(path, w, h, c, STBI_rgb_alpha);
}

// create a texture struct
texture new_texture(char* path, char* name) {
  texture t;
  glGenTextures(1, &(t.gl_ptr));
  t.pixel_buf = create_image(path, &(t.w), &(t.h), &(t.c));
  t.name      = name;
  return t;
}

// bind a given texture to a shader program
void bind_texture(texture t, unsigned int program) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t.gl_ptr);
  glTexImage2D(
    GL_TEXTURE_2D,    // target
    0,                // level
    GL_RGBA,          // internal format
    t.w, t.h,         // width, height
    0,                // border
    GL_RGB,           // format
    GL_UNSIGNED_BYTE, // type
    NULL              // data
  );
  // bind it to the uniform
  glUniform1i(glGetUniformLocation(program, "tex"), 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexSubImage2D(
    GL_TEXTURE_2D,               // target
    0, 0, 0,                     // level, x&y offset
    t.w, t.h,                    // width, height
    GL_RGBA,                     // format
    GL_UNSIGNED_INT_8_8_8_8_REV, // type
    t.pixel_buf                  // pixels
  );
  // blend
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
