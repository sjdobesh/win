/*=================*
 *                 *
 *    uniform.h    *
 *                 *
 *===================================*
 * samantha jane                     *
 * handles uniform data and updates. *
 *==========================================================================80*/

#ifndef _UNIFORM_H_
#define _UNIFORM_H_

/* indicate type in the union */
enum uniform_type { INT, FLOAT };

/* update function pointers */
typedef int (*int_update_func)(int);
typedef float (*float_update_func)(float);

/* data structure for the setting and updating of uniform shader variables */
typedef struct uniform {
  char* name;
  uniform_type type;
  union {
    int ival;
    float fval; 
  };
  union {
    int_update_func iUpdate;
    float_update_func fUpdate;
  };
} uniform;

/* update a uniform one step */
uniform update_uniform(uniform u);
/* print uniform contents */
void uniform_print(uniform u);
void uniform_tab_print(uniform u, unsigned tab_level);

#endif
