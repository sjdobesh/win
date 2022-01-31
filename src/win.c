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
  program p = load_new_program(
    "default",
    "../shaders/default.vert", // vert path
    "../shaders/default.frag" // frag path
  );

  window.prog = &p;

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
    "gec",
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
  glGenVertexArrays(1, &(w->prog->vao));
  glBindVertexArray(w->prog->vao);
}

//------------------------------------
// initialize an OpenGL geometry data
//------------------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
void init_win_geometry(win* w) {
  // Screen Quad //-------------------------
  GLfloat verts[4][4] = {
    { 0.0, 0.0, 0.0, 0.0 }, // TL
    { 1.0, 0.0, 1.0, 0.0 }, // TR
    { 1.0, 1.0, 1.0, 1.0 }, // BR
    { 0.0, 1.0, 0.0, 1.0 }, // BL
  };
  // quad indicies
  GLint indicies[] = {
    0, 1, 2, 0, 2, 3
  };
  // BIND BUFFERS //
  // vertex buffer
  glGenBuffers(1, &(w->prog->vbo));
  glBindBuffer(GL_ARRAY_BUFFER, w->prog->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  // element buffer
  glGenBuffers(1, &(w->prog->ebo));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, w->prog->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
  // bind vertex position attribute
  GLint pos_attr_loc = glGetAttribLocation(w->prog->gl_ptr, "in_Position");
  glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(pos_attr_loc);
  // bind vertex texture coordinate attribute
  GLint tex_attr_loc = glGetAttribLocation(w->prog->gl_ptr, "in_Texcoord");
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
  glDetachShader(w->prog->gl_ptr, w->prog->vert->gl_ptr);
  glDetachShader(w->prog->gl_ptr, w->prog->frag->gl_ptr);
  glDeleteProgram(w->prog->gl_ptr);
  glDeleteShader(w->prog->vert->gl_ptr);
  glDeleteShader(w->prog->frag->gl_ptr);
  // glDeleteTextures(1, &(w->tex));
  glDeleteBuffers(1, &(w->prog->ebo));
  glDeleteBuffers(1, &(w->prog->vbo));
  glDeleteVertexArrays(1, &(w->prog->vao));
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
  w->prog = &p;
  init_win_shaders(w);
  init_win_geometry(w);
}



// MAIN //----------------------------------------------------------------------
void prompt() {
  scanf("> ");
}
void welcome() {
  printf("\n----------------------------\n");
  printf("WIN -- opengl window manager\n");
  printf("----------------------------\n");
  printf("author: samantha dobesh -- 2022\n");
  printf("press 'h' for help\n\n");
}
void test() {
  printf("initializing window...\n");
  win w = init_win(200, 200);
  printf("window initialized.\n");

  printf("loading shader...\n");
  load_program(&w, "../shaders/default.vert", "../shaders/default.frag");
  printf("shader loaded.\n");

  printf("load texture...\n");
  texture t = new_texture("../textures/texture.jpg", "default", 100, 100);
  bind_texture(t, w.prog->gl_ptr);
  printf("texture loaded.\n");

  printf("rendering window\n");
  win_render(w);
  printf("window rendered\n");

  char junk[10];
  scanf("%s", junk);

  /*
  printf("cleaning window\n");
  win_clean(&w);
  printf("window cleaned\n");
  */
}
int main() {
  welcome();
  prompt();
  char user_choice[10];
  scanf("%s", user_choice);
  test();
}
//------------------------------------------------------------------------------





















// //-------------------------
// // load an image with stbi
// //-------------------------
// // I: texture data - tex_parameters*
// // O: exit code    - int
// //------------------------------------------------------------------------------
// int load_image(tex_parameters* t) {
//   t->pixel_buf = (char*)stbi_load(
//     t->path,
//     &(t->w),
//     &(t->h),
//     &(t->c),
//     STBI_rgb_alpha
//   );
//   return 0;
// }

//------------------------------------------------------
// project square into normalized coordinates
//------------------------------------------------------
// I: (r)ectangle  - rect*
//    (win_w)idth  - float
//    (hin_h)eigth - float
// O: rectangle    - rect
//------------------------------------------------------------------------------

//----------------------------
// Reset OpenGL geometry data
//----------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
// int update_win_geometry(win* w) {
//   // Screen Quad //-------------------------
//   aaquad proj = square2norm(p->s->box, p->w, p->h);
//   GLfloat verts[4][4] = {
//     { proj.pos.x, proj.pos.y, 0.0, 0.0 }, // TL
//     { proj.dim.x, proj.pos.y, 1.0, 0.0 }, // TR
//     { proj.dim.x, proj.dim.y, 1.0, 1.0 }, // BR
//     { proj.pos.x, proj.dim.y, 0.0, 1.0 }, // BL
//   };
//   // vertex buffer
//   glGenBuffers(1, &(p->vbo));
//   glBindBuffer(GL_ARRAY_BUFFER, p->vbo);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
//   // bind vertex position attribute
//   GLint pos_attr_loc = glGetAttribLocation(p->shader_prog, "in_Position");
//   glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
//   glEnableVertexAttribArray(pos_attr_loc);
//   return 0;
// }

// initialize OpenGL textures
// int init_win_textures(win* w) {
//   // load a texture with texture parameters (t)
//   // load_image(p->t);
//   // make a texture
//   glGenTextures(1, &(p->tex));
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, p->tex);
//   glTexImage2D(
//     GL_TEXTURE_2D,    // target
//     0,                // level
//     GL_RGBA,          // internal format
//     p->t->w, p->t->h, // width, height
//     0,                // border
//     GL_RGB,           // format
//     GL_UNSIGNED_BYTE, // type
//     NULL              // data
//   );
//   // bind it to the uniform
//   glUniform1i(glGetUniformLocation(p->shader_prog, "tex"), 0);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexSubImage2D(
//     GL_TEXTURE_2D,               // target
//     0, 0, 0,                     // level, x&y offset
//     p->t->w, p->t->h,            // width, height
//     GL_RGBA,                     // format
//     GL_UNSIGNED_INT_8_8_8_8_REV, // type
//     p->t->pixel_buf              // pixels
//   );
//   // blend
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   return 0;
// }

//------------------------
// update OpenGL textures
//------------------------
// I: parameters  - win_parameters*
// O: exit code   - int
//------------------------------------------------------------------------------
// int update_win_textures(win_parameters* p) {
//   // make a texture
//   glGenTextures(1, &(p->tex));
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_2D, p->tex);
//   glTexImage2D(
//     GL_TEXTURE_2D,    // target
//     0,                // level
//     GL_RGBA,          // internal format
//     p->t->w, p->t->h, // width, height
//     0,                // border
//     GL_RGB,           // format
//     GL_UNSIGNED_BYTE, // type
//     NULL              // data
//   );
//   // bind it to the uniform
//   glUniform1i(glGetUniformLocation(p->shader_prog, "tex"), 0);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexSubImage2D(
//     GL_TEXTURE_2D,               // target
//     0, 0, 0,                     // level, x&y offset
//     p->t->w, p->t->h,            // width, height
//     GL_RGBA,                     // format
//     GL_UNSIGNED_INT_8_8_8_8_REV, // type
//     p->t->pixel_buf              // pixels
//   );
//   // blend
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   return 0;
// }

