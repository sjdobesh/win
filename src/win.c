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
#include "init.h"
#include "texture.h"
#include "win.h"
#include "gl.h"

// initializations & cleaning //-------------------------------------------------

// initialize a window with sdl and opengl context
win init_win(int h, int w) {
  printf("initalizing sdl...\n");
  SDL_Window* sdl_window = init_sdl(w, h);
  printf("initalizing context...\n");
  SDL_GLContext sdl_context = init_context(sdl_window);

  win window = {
    .window = sdl_window,
    .context = sdl_context,
    .w = w, .h = h
  };

  // TEMPORARY WAY TO LOAD DEFAULT SHADER
  printf("loading shader...\n");
  window.prog = load_new_program(
    "default",
    "../shaders/default.vert",
    "../shaders/default.frag",
    0, 0, h, w
  );

  printf("initalizing shader...\n");
  bind_vao(window.prog.vao);
  return window;
}

// render opengl in sdl_window
void render_win(win w) {
  // draw elements
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  // swap frame
  SDL_GL_SwapWindow(w.window);
}

// free OpenGL data
void clean_win(win *w) {
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

// printing //---------------------------------------------------------

void print_win(win w) {
  printf("win [ \n");
  printf("  w x h : %d x %d\n", w.w, w.h);
  print_program(w.prog);
  printf("]\n");
}

// welcome
//--------------------------------------------------------------------

void welcome() {
  printf("\n------------------------------------\n");
  printf("win -- an opengl powered window manager\n");
  printf("------------------------------------\n");
  printf("author: samantha jane -- 2022\n\n");
}
