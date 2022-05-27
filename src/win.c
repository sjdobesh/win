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
#include "msleep.h"
#include "texture.h"
#include "win.h"

// initializations //-----------------------------------------------------------

// initialize a window with sdl and opengl context.
// for now load a default shader and init the vao
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

void screentonormalized(float *pos_x, float *pos_y, float *dim_x, float *dim_y,
                        int w, int h) {
  /// convert range from 0 to w to -1 to 1
  // 2x/w - 1, -2x/h + 1
  printf("win: %dx%d\n", w, h);
  printf("pos : %.2f, %.2f dim : %.2f, %.2f\n", *pos_x, *pos_y, *dim_x, *dim_y);
  *pos_x = ((2.0 / (float)w) * *pos_x) - 1.0;
  *pos_y = ((-2.0 / (float)h) * *pos_y) + 1.0;
  *dim_x = ((2.0 / (float)w) * *dim_x) - 1.0;
  *dim_y = ((-2.0 / (float)h) * *dim_y) + 1.0;
  printf("pos : %.2f, %.2f dim : %.2f, %.2f\n", *pos_x, *pos_y, *dim_x, *dim_y);
}

// generate and bind vertex array object (vao) to the program in the window
// struct
void bind_vao(win *w) {
  glGenVertexArrays(1, &(w->prog.vao));
  glBindVertexArray(w->prog.vao);
}

void bind_vbo(win *w, const float *vertex_array, int length) {
  // vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, w->prog.vbo);
  glBufferData(GL_ARRAY_BUFFER, length * sizeof(GLfloat), vertex_array,
               GL_STATIC_DRAW);
}

void bind_ebo(win *w, const int *indicies, int length) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, w->prog.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(GLint), indicies,
               GL_STATIC_DRAW);
}

void bind_vertex_attributes(win *w) {
  // vertex position
  GLint pos_attr_loc = glGetAttribLocation(w->prog.gl_ptr, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(pos_attr_loc);
  // texture coordinate
  GLint tex_attr_loc = glGetAttribLocation(w->prog.gl_ptr, "in_Texcoord");
  glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(tex_attr_loc);
}

// initialize an OpenGL geometry data
void init_geometry(win *w) {
  // get normalized coords
  float p1 = w->prog.pos_x;
  float p2 = w->prog.pos_y;
  float p3 = w->prog.pos_x + w->prog.dim_x;
  float p4 = w->prog.pos_y + w->prog.dim_y;
  screentonormalized(&p1, &p2, &p3, &p4, w->w, w->h);

  // convert to normalized screen quad
  GLfloat verts[4][4] = {
      {p1, p2, 1.0, 0.0},
      {p3, p2, 0.0, 0.0},
      {p3, p4, 0.0, 1.0},
      {p1, p4, 1.0, 1.0},
  };
  // quad indicies
  GLint indicies[] = {0, 1, 2, 0, 2, 3};
  // GENERATE & BIND BUFFERS //
  glGenBuffers(1, &(w->prog.vbo));
  glGenBuffers(1, &(w->prog.ebo));
  bind_vbo(w, (const float *)verts, 4);
  bind_ebo(w, (const int *)indicies, 6);

  // bind attributes
  // position
  GLint pos_attr_loc = glGetAttribLocation(w->prog.gl_ptr, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(pos_attr_loc);
  // texture coordinate
  GLint tex_attr_loc = glGetAttribLocation(w->prog.gl_ptr, "in_Texcoord");
  glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(tex_attr_loc);
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

void color_red() {
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void color_green() {
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void color_blue() {
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

// Render openGL in SDL_Window
void win_render(win w) {
  // set color
  color_red();
  // draw elements
  printf("draw\n");
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  // swap frame
  printf("swap\n");
  SDL_GL_SwapWindow(w.window);
}

// draw an arbitrary set of vertices to the screen
void debug_draw(int *vertex_array, int length) {
  glDrawArrays(GL_LINES, 0, length);
}

// load a program from paths and init vao and geometry
void load_program(win *w, char *vert_path, char *frag_path, int pos_x,
                  int pos_y, int dim_x, int dim_y) {
  program p = load_new_program("default", vert_path, frag_path, pos_x, pos_y,
                               dim_x, dim_y);
  use_program(&p);
  w->prog = p;
  bind_vao(w);
  init_geometry(w);
}

void print_win(win w) {
  printf("win [ \n");
  printf("  w x h : %d x %d\n", w.w, w.h);
  print_program(w.prog);
  printf("]\n");
}

void new_sprite(win *w, char *texture_path, int pos_x, int pos_y, int dim_x,
                int dim_y) {
  // default sprite program
  load_program(w, "../shaders/default.vert", "../shaders/default.frag", pos_x,
               pos_y,       // pos
               dim_x, dim_y // dim
  );
  w->prog.tex = new_texture(texture_path, "default");
  bind_texture(w->prog.tex, w->prog.gl_ptr);
}

// MAIN //----------------------------------------------------------------------

void welcome() {
  printf("\n----------------------------\n");
  printf("win -- opengl window manager\n");
  printf("----------------------------\n");
  printf("author: samantha dobesh -- 2022\n\n");
}

void pause() {
  char s[1];
  printf("enter anything to continue: ");
  scanf("%s", s);
}

void texture_test() {
  printf("running test...\n");
  win w = init_win(800, 800);
  new_sprite(&w, "../textures/texture.jpg", w.w / 2.0, w.h / 2.0, 200, 200);
  win_render(w);
  pause();
  printf("\nexiting...\n");
  win_clean(&w);
}

void debug_test() {
  printf("running test...\n");
  win w = init_win(800, 800);
  drawShape();
  win_render(w);
  pause();
  printf("\nexiting...\n");
  win_clean(&w);
}

int main() {
  welcome();
  printf("running test...\n");
  texture_test();
}
