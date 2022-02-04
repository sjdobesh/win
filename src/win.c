//=============//
//             //
//    WIN.C    //
//             //
//======================================//
// Handle SDL window and openGL context //
//============================================================================80

// SDL & OpenGL
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// custom module
#include "win.h"
#include "texture.h"

// INITIALIZATIONS //-----------------------------------------------------------

// initialize a window with a parameter struct
win init_win(int h, int w) {
  win window = {.w = w, .h = h};
  printf("initalizing sdl...\n");
  init_sdl(&window);
  printf("initalizing context...\n");
  init_context(&window);

  printf("loading shader...\n");
  // TEMPORARY WAY TO LOAD DEFAULT SHADER
  window.prog = load_new_program(
    "default",
    "../shaders/default.vert", // vert path
    "../shaders/default.frag" // frag path
  );

  printf("initalizing shader...\n");
  init_win_shaders(&window);
  return window;
}

// initialize an SDL window
void init_sdl(win* w) {
  // init SDL video
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "[ERROR] failed to initialize SDL video\n");
    exit(EXIT_FAILURE);
  }
  // create window
  SDL_Window* window = SDL_CreateWindow(
    "win",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    w->w, w->h,
    SDL_WINDOW_OPENGL
  );
  if (window == NULL) {
    fprintf(stderr, "[ERROR] failed to create main window.\n");
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  w->window = window;
}

// initialize an OpenGL context (& GLEW)
void init_context(win* w) {
  // set gl attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // init rendering context
  SDL_GLContext context = SDL_GL_CreateContext(w->window);
  if (context == NULL) {
    fprintf(stderr, "[ERROR] context creation failed\n");
    SDL_DestroyWindow(w->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  SDL_GL_SetSwapInterval(1); // vsync
  // GLEW
  glewExperimental = GL_TRUE; // OpenGL 3.+
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "[ERROR] GLEW initialization failed\n");
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(w->window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  w->context = context;
}

// initialize an OpenGL shader program
void init_win_shaders(win* w) {
  // bind vao
  glGenVertexArrays(1, &(w->prog.vao));
  glBindVertexArray(w->prog.vao);
}

// initialize an OpenGL geometry data
void init_win_geometry(win* w) {
  // screen quad
  GLfloat verts[4][4] = {
    { -1.0, -1.0, 0.0, 1.0 }, // TL
    {  1.0, -1.0, 1.0, 1.0 }, // TR
    {  1.0,  1.0, 1.0, 0.0 }, // BR
    { -1.0,  1.0, 0.0, 0.0 }, // BL
  };
  // quad indicies
  GLint indicies[] = {
    0, 1, 2, 0, 2, 3
  };
  // BIND BUFFERS //
  // vertex buffer
  glGenBuffers(1, &(w->prog.vbo));
  glBindBuffer(GL_ARRAY_BUFFER, w->prog.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  // element buffer
  glGenBuffers(1, &(w->prog.ebo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, w->prog.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
  // bind vertex position attribute
  GLint pos_attr_loc = glGetAttribLocation(w->prog.gl_ptr, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(pos_attr_loc);
  // bind vertex texture coordinate attribute
  GLint tex_attr_loc = glGetAttribLocation(w->prog.gl_ptr, "in_Texcoord");
  glVertexAttribPointer(
    tex_attr_loc,
    2,
    GL_FLOAT,
    GL_FALSE,
    4 * sizeof(GLfloat),
    (void*)(2 * sizeof(GLfloat))
  );
  glEnableVertexAttribArray(tex_attr_loc);
}

// free OpenGL data
void win_clean(win* w) {
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

// Render openGL in SDL_Window
void win_render(win w) {
  // set color
  printf("color\n");
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  // draw elements
  printf("draw\n");
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  // swap frame
  printf("swap\n");
  SDL_GL_SwapWindow(w.window);
}

void load_program(win* w, char* vert_path, char* frag_path) {
  program p = load_new_program("default", vert_path, frag_path);
  use_program(&p);
  w->prog = p;
  init_win_shaders(w);
  init_win_geometry(w);
}

void print_win(win w) {
  printf("win [ \n");
  printf("  w x h : %d x %d\n", w.w, w.h);
  print_program(w.prog);
  printf("]\n");
}



// MAIN //----------------------------------------------------------------------

void welcome() {
  printf("\n----------------------------\n");
  printf("WIN -- opengl window manager\n");
  printf("----------------------------\n");
  printf("author: samantha dobesh -- 2022\n\n");
}
void test() {
  printf("initializing window...\n");
  win w = init_win(800, 800);
  printf("window initialized.\n");

  printf("loading shader...\n");
  load_program(&w, "../shaders/default.vert", "../shaders/default.frag");
  printf("shader loaded.\n");

  printf("load texture...\n");
  texture t = new_texture("../textures/texture.jpg", "default");
  bind_texture(t, w.prog.gl_ptr);
  printf("texture loaded.\n");

  printf("rendering window\n");
  win_render(w);
  printf("window rendered\n");

  printf("enter anything to quit:");
  char junk[10];
  scanf("%s", junk);

  printf("\ncleaning window\n");
  win_clean(&w);
  printf("window cleaned\n");
}

int main() {
  welcome();
  printf("running test...\n");
  test();
}
