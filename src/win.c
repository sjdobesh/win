/*=============*
 *             *
 *    win.c    *
 *             *
 *======================================*
 * samantha jane                        *
 * handle SDL window and opengl context *
 *==========================================================================80*/

#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "sdlinit.h"
#include "texture.h"
#include "win.h"
#include "gl.h"

/* initialize a window with sdl and opengl context from its size */
void init_win(win* w) {
  w->window = init_sdl(w->w, w->h);
  w->context = init_context(sdl_window);
}

/* render opengl in sdl_window */
void render_win(win w) {
  /* compile with sensible defaults */
  compile_shader(w.vert, load_shader_code(w.vert.path));
  compile_shader(w.frag, load_shader_code(w.frag.path));
  /* link */
  w.prog.gl_ptr = glCreateProgram();
  link_program(w.prog.gl_ptr, w.vert.gl_ptr, w.frag.gl_ptr);
  /* bind */
  glUseProgram(p->gl_ptr);
  bind_vao(window.prog.vao);
  /* draw elements */
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
  /* swap frame */
  SDL_GL_SwapWindow(w.window);
}

// free OpenGL data
void clean_win(win *w) {
  // gl program data
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

// // debug print
// void print_win(win w) {
//   printf("win [ \n");
//   printf("  w x h : %d x %d\n", w.w, w.h);
//   print_program(w.prog);
//   printf("]\n");
// }
// 
// // welcome
// void welcome() {
//   printf("\n----------------------------------------\n");
//   printf("win -- an opengl powered window manager\n");
//   printf("----------------------------------------\n");
//   printf("author: samantha jane -- 2022\n\n");
// }
