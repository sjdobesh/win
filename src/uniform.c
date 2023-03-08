/*=================*
 *                 *
 *    uniform.c    *
 *                 *
 *===================================*
 * samantha jane                     *
 * handles uniform data and updates. *
 *==========================================================================80*/

#include <stdio.h>
#include "uniform.h"

/* update a uniform according to its own function.
 * if the uniform is not updateable, an error is
 * printed to stderr and the uniform is returned unchanged
 */
uniform update_uniform(uniform u) {
  /* check for update function */
  if (!u.iUpdate) {
    fprintf(stderr, "[ERROR] attempted to update a static uniform.\n");
    return u;
  }
  /* update by type */
  switch (u.type) {
    case INT:
      u.ival = u.iUpdate(u.ival);
      break;
    case FLOAT:
      u.fval = u.fUpdate(u.fval);
      break;
    default: break;
  }
  return u;
}

/* local private indent function */
void tab_print(unsigned tabs) {
  for (int i = 0; i < tabs; i++)
    printf("  ");

/* print with no indent*/
void uniform_print(uniform u) {
  uniform_tab_print(u, 0);
}

/* print and indent to a given level */
void uniform_tab_print(uniform u, unsigned tab_level) {
  tab_print(tab_level);
  printf("UNIFORM [\n");
  tab_print(tab_level);
  printf("  name: %s\n", u.name);
  tab_print(tab_level);
  printf("  type: ");
  switch (u.type) {
    case INT:
      printf("INT\n");
      break;
    case FLOAT:
      printf("FLOAT\n");
      break;
    default: break;
  }
  tab_print(tab_level);
  printf("  update: %s", u.iUpdate ? "yes\n" : "no\n");
  tab_print(tab_level);
  printf("]\n");
}

