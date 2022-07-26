// init.c
// creates sdl and opengl context

// sdl & opengl
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>

// initialize an SDL window and return it
SDL_Window* init_sdl(int w, int h) {
  // init SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "[ERROR] failed to initialize SDL video\n");
    exit(EXIT_FAILURE);
  }
  // create window
  SDL_Window *window =
      SDL_CreateWindow("win", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       w, h, SDL_WINDOW_OPENGL);
  if (window == NULL) {
    fprintf(stderr, "[ERROR] failed to create window in init_sdl.\n");
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  return window;
}

// initialize an OpenGL context (& GLEW)
SDL_GLContext init_context(SDL_Window* w) {
  // set gl attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // init rendering context
  SDL_GLContext context = SDL_GL_CreateContext(w);
  if (context == NULL) {
    fprintf(stderr, "[ERROR] context creation failed in init_context.\n");
    SDL_DestroyWindow(w);
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
    SDL_DestroyWindow(w);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  return context;
}
