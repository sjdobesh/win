//=============//
//             //
//    win.c    //
//             //
//======================================//
// samantha jane                        //
// handle SDL window and opengl context //
//============================================================================80

// std
#include <math.h>
#include <time.h>

// sdl & opengl
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>

// custom module
#include "texture.h"
#include "win.h"

// initializations & cleaning //-------------------------------------------------

// initialize a window with sdl and opengl context
win init_win(int h, int w) {
  win window = {.w = w, .h = h};
  printf("initalizing sdl...\n");
  init_sdl(&window);
  printf("initalizing context...\n");
  init_context(&window);

  // TEMPORARY WAY TO LOAD DEFAULT SHADER
  printf("loading shader...\n");
  window.prog = load_new_program("default", "../shaders/default.vert",
                                 "../shaders/default.frag", 0, 0, h, w);

  printf("initalizing shader...\n");
  bind_vao(&window);
  return window;
}

// initialize an SDL window
void init_sdl(win *w) {
  // init SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "[ERROR] failed to initialize SDL video\n");
    exit(EXIT_FAILURE);
  }
  // create window
  SDL_Window *window =
      SDL_CreateWindow("win", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       w->w, w->h, SDL_WINDOW_OPENGL);
  if (window == NULL) {
    fprintf(stderr, "[ERROR] failed to create window in init_sdl.\n");
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  w->window = window;
}

// initialize an OpenGL context (& GLEW)
void init_context(win *w) {
  // set gl attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // init rendering context
  SDL_GLContext context = SDL_GL_CreateContext(w->window);
  if (context == NULL) {
    fprintf(stderr, "[ERROR] context creation failed in init_context.\n");
    SDL_DestroyWindow(w->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  SDL_GL_SetSwapInterval(1); // vsync
  // GLEW
  glewExperimental = GL_TRUE; // OpenGL 3.+
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "[ERROR] GLEW initialization failed in init_context.\n");
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(w->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  w->context = context;
}

// render opengl in sdl_window
void win_render(win w) {
  // draw elements
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  // swap frame
  SDL_GL_SwapWindow(w.window);
}

// free OpenGL data
void win_clean(win *w) {
  // clean out gl program data
  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDetachShader(w->prog.gl_ptr, w->prog.vert.gl_ptr);
  glDetachShader(w->prog.gl_ptr, w->prog.frag.gl_ptr);
  glDeleteProgram(w->prog.gl_ptr);
  glDeleteShader(w->prog.vert.gl_ptr);
  glDeleteShader(w->prog.frag.gl_ptr);
  // glDeleteTextures(1, &(w->tex));
  glDeleteBuffers(1, &(w->prog.ebo));
  glDeleteBuffers(1, &(w->prog.vbo));
  glDeleteVertexArrays(1, &(w->prog.vao));
  // sdl items
  SDL_GL_DeleteContext(w->context);
  SDL_DestroyWindow(w->window);
  SDL_Quit();
}
