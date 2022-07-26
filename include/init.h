#ifndef _INIT_H_
#define _INIT_H_
// SDL & opengl
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
// initialize an SDL window and return it
SDL_Window* init_sdl(int w, int h);
SDL_GLContext init_context(SDL_Window * w);
#endif
