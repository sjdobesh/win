//=========================//
//                         //
//        uniform.h        //
//                         //
//===================================//
// handles uniform data and updates. //
//============================================================================80

#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#include "time_utils.h"
#include <stdio.h>
#include <stdlib.h>

// indicate type last placed into the unions
enum uniform_type { INT, FLOAT, V2, V3, V4, TEX };

// update function pointers
typedef int (*intUpdateFunc)(int);
typedef float (*floatUpdateFunc)(float);
typedef float *(*vecUpdateFunc)(float *);

// data structure for the setting and updating of uniform shader variables
typedef struct Uniform {
  char *name;
  union {
    int ival;
    float fval;
    float *fpval;
  };
  unsigned int type;
  int dynamic;
  union {
    intUpdateFunc iUpdate;
    floatUpdateFunc fUpdate;
    vecUpdateFunc vecUpdate;
  };
} Uniform;

// prototypes
void update_uniform(Uniform *u);
void uniform_print(Uniform u);

// static
Uniform new_i_uniform(char *name, int val);
Uniform new_f_uniform(char *name, float val);
Uniform new_v2_uniform(char *name, float x, float y);
Uniform new_v3_uniform(char *name, float x, float y, float z);
Uniform new_v4_uniform(char *name, float x, float y, float z, float w);
Uniform new_tex_uniform(char *name, char *path);

// dynamic
Uniform new_di_uniform(char *name, int val, int (*intUpdateFunc)(int));
Uniform new_df_uniform(char *name, float val, float (*floatUpdateFunc)(float));
Uniform new_dv2_uniform(char *name, float x, float y,
                        float *(*vecUpdateFunc)(float *));
Uniform new_dv3_uniform(char *name, float x, float y, float z,
                        float *(*vecUpdateFunc)(float *));
Uniform new_dv4_uniform(char *name, float x, float y, float z, float w,
                        float *(*vecUpdateFunc)(float *));

Uniform time_uniform();
Uniform resolution_uniform();

void set_uniform(Uniform u);


#endif
