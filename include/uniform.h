//=========================//
//                         //
//        uniform.h        //
//                         //
//===================================//
// handles uniform data and updates. //
//============================================================================80

#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#include <stdio.h>
#include <stdlib.h>
#include "time_utils.h"

// indicate type last placed into the unions
enum uniform_type { INT, FLOAT, V2, V3, V4, SAMPLER2D };

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

// constructors //--------------------------------------------------------------

// integers //

// create a new static integer uniform
Uniform new_i_uniform(char *name, int val) {
  Uniform u = {.name = name, .ival = val, .type = INT, .dynamic = 0};
  return u;
}

// create a new dynamic integer uniform
Uniform new_di_uniform(char *name, int val, int (*intUpdateFunc)(int)) {
  Uniform u = {.name = name,
               .ival = val,
               .type = INT,
               .dynamic = 1,
               .iUpdate = intUpdateFunc};
  return u;
}

// floats //

// create a new static float uniform
Uniform new_f_uniform(char *name, float val) {
  Uniform u = {.name = name, .fval = val, .type = FLOAT, .dynamic = 0};
  return u;
}

// create a new dynamic float uniform
Uniform new_df_uniform(char *name, float val, float (*floatUpdateFunc)(float)) {
  Uniform u = {.name = name,
               .fval = val,
               .type = FLOAT,
               .dynamic = 1,
               .fUpdate = floatUpdateFunc};
  return u;
}

// vector 2 //

// create a new static vector 2 uniform
Uniform new_v2_uniform(char *name, float x, float y) {
  float val[2] = {x, y};
  Uniform u = {.name = name, .fpval = val, .type = V2, .dynamic = 0};
  return u;
}

// create a new dynamic vector 2 uniform
Uniform new_dv2_uniform(char *name, float x, float y,
                        float *(*vecUpdateFunc)(float *)) {
  float val[3] = {x, y};
  Uniform u = {.name = name,
               .fpval = val,
               .type = V2,
               .dynamic = 1,
               .vecUpdate = vecUpdateFunc};
  return u;
}

// vector 3 //

// create a new static vector 3 uniform
Uniform new_v3_uniform(char *name, float x, float y, float z) {
  float val[3] = {x, y, z};
  Uniform u = {.name = name, .fpval = val, .type = V3, .dynamic = 0};
  return u;
}

// create a new dynamic vector 3 uniform
Uniform new_dv3_uniform(char *name, float x, float y, float z,
                        float *(*vecUpdateFunc)(float *)) {
  float val[3] = {x, y, z};
  Uniform u = {.name = name,
               .fpval = val,
               .type = V3,
               .dynamic = 1,
               .vecUpdate = vecUpdateFunc};
  return u;
}

// vector 4 //

// create a new static vector 4 uniform
Uniform new_v4_uniform(char *name, float x, float y, float z, float w) {
  float val[4] = {x, y, z, w};
  Uniform u = {.name = name, .fpval = val, .type = V4, .dynamic = 0};
  return u;
}

// create a new dynamic vector 4 uniform
Uniform new_dv4_uniform(char *name, float x, float y, float z, float w,
                        float *(*vecUpdateFunc)(float *)) {
  float val[4] = {x, y, z, w};
  Uniform u = {.name = name,
               .fpval = val,
               .type = V4,
               .dynamic = 1,
               .vecUpdate = vecUpdateFunc};
  return u;
}

// update a uniform according to its own function.
// if the uniform is not dynamic, an error is printed
// to stderr and the uniform is returned unchanged
void update_uniform(Uniform* u) {
  // check for incorrect update
  if (!u->dynamic) {
    fprintf(stderr, "[ERROR] attempted to update a static uniform.\n");
    return;
  }
  // update by type
  switch (u->type) {
  case INT:
    u->ival = u->iUpdate(u->ival);
    break;
  case FLOAT:
    u->fval = u->fUpdate(u->fval);
    break;
  default:
    u->fpval = u->vecUpdate(u->fpval);
    break;
  }
  return;
}

float update_time(float x) {
  return mseconds();
}

void test() {
  Uniform u = new_df_uniform(
      "float",
      0.0,
      &update_time
  );
  update_uniform(&u);
}

void uniform_print(Uniform u) {
  printf("Uniform %s\n", u.name);
  printf("  - type: ");
  switch(u.type) {
  case INT:
    printf("INT\n");
    printf("  - val: %d", u.ival);
    break;
  case FLOAT:
    printf("FLOAT\n");
    printf("  - val: %.2f", u.fval);
    break;
  case V2:
    printf("V2\n");
    printf("  - val: ");
    for (int i = 0; i < 2; i++) {
      printf("%.2f ", u.fpval[i]);
    }
    printf("\n");
    break;
  case V3:
    printf("V3\n");
    printf("  - val: ");
    for (int i = 0; i < 3; i++) {
      printf("%.2f ", u.fpval[i]);
    }
    printf("\n");
    break;
  case V4:
    printf("V4\n");
    printf("  - val: ");
    for (int i = 0; i < 4; i++) {
      printf("%.2f ", u.fpval[i]);
    }
    printf("\n");
    break;
  }
  printf("  - dynamic: %d\n", u.dynamic);
  if (u.dynamic) {
    printf("  - update_function: %pf", u.fUpdate);
  }
}

#endif
