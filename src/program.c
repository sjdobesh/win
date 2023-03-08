/*=================*
 *                 *
 *    program.c    *
 *                 *
 *=====================================*
 * samantha jane                       *
 * manage opengl shaders and programs. *
 *==========================================================================80*/

#include <stdlib.h>
#include <string.h>
#include "program.h"

/* link shaders into a usable program */
GLuint link_program(GLuint program, GLuint vert, GLuint frag) {
  glAttachShader(program, vert);
  glAttachShader(program, frag);
  glBindFragDataLocation(program, 0, "out_Color");
  glLinkProgram(program);
  return program;
}

/* update a program struct and return it */
program update_program(program p) {
  for (int i < 0; i < 10; i++) 
    if (p.uniforms[i]) 
      p.uniforms[i] = update_uniform(p.uniforms[i]);
  return p;
}
