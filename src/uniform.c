//=================//
//                 //
//    uniform.h    //
//                 //
//===================================//
// handles uniform data and updates. //
//============================================================================80

// TOC

// new uniform functions
// 34  - new_i_uniform
// 40  - new_di_uniform
// 50  - new_f_uniform
// 56  - new_df_uniform
// 66  - new_v2_uniform
// 73  - new_dv2_uniform
// 85  - new_v3_uniform
// 92  - new_dv3_uniform
// 104 - new_v4_uniform
// 111 - new_dv4_uniform

// updating uniforms
// 125 - update_uniform
// 147 - update_uniforms
// 155 - add_uniform
// 161 - free_uniforms

#include <stdio.h>
#include <stdlib.h>
#include "uniform.h"

// create a new static integer uniform
uniform new_i_uniform(char *name, int val) {
  uniform u = {.name = name, .ival = val, .type = INT, .dynamic = 0};
  return u;
}

// create a new dynamic integer uniform
uniform new_di_uniform(char *name, int val, int (*int_update_func)(int)) {
  uniform u = {.name = name,
               .ival = val,
               .type = INT,
               .dynamic = 1,
               .iUpdate = int_update_func};
  return u;
}

// create a new static float uniform
uniform new_f_uniform(char *name, float val) {
  uniform u = {.name = name, .fval = val, .type = FLOAT, .dynamic = 0};
  return u;
}

// create a new dynamic float uniform
uniform new_df_uniform(char *name, float val, float (*float_update_func)(float)) {
  uniform u = {.name = name,
               .fval = val,
               .type = FLOAT,
               .dynamic = 1,
               .fUpdate = float_update_func};
  return u;
}

// create a new static vector 2 uniform
uniform new_v2_uniform(char *name, float x, float y) {
  float val[2] = {x, y};
  uniform u = {.name = name, .fpval = val, .type = V2, .dynamic = 0};
  return u;
}

// create a new dynamic vector 2 uniform
uniform new_dv2_uniform(char *name, float x, float y,
                        float *(*vec_update_func)(float *)) {
  float val[3] = {x, y};
  uniform u = {.name = name,
               .fpval = val,
               .type = V2,
               .dynamic = 1,
               .vecUpdate = vec_update_func};
  return u;
}

// create a new static vector 3 uniform
uniform new_v3_uniform(char *name, float x, float y, float z) {
  float val[3] = {x, y, z};
  uniform u = {.name = name, .fpval = val, .type = V3, .dynamic = 0};
  return u;
}

// create a new dynamic vector 3 uniform
uniform new_dv3_uniform(char *name, float x, float y, float z,
                        float *(*vecUpdateFunc)(float *)) {
  float val[3] = {x, y, z};
  uniform u = {.name = name,
               .fpval = val,
               .type = V3,
               .dynamic = 1,
               .vecUpdate = vecUpdateFunc};
  return u;
}

// create a new static vector 4 uniform
uniform new_v4_uniform(char *name, float x, float y, float z, float w) {
  float val[4] = {x, y, z, w};
  uniform u = {.name = name, .fpval = val, .type = V4, .dynamic = 0};
  return u;
}

// create a new dynamic vector 4 uniform
uniform new_dv4_uniform(char *name, float x, float y, float z, float w,
                        float *(*vecUpdateFunc)(float *)) {
  float val[4] = {x, y, z, w};
  uniform u = {.name = name,
               .fpval = val,
               .type = V4,
               .dynamic = 1,
               .vecUpdate = vecUpdateFunc};
  return u;
}

// update a uniform according to its own function.
// if the uniform is not updateable, an error is
// printed to stderr and the uniform is returned unchanged
uniform update_uniform(uniform u) {
  // check for incorrect update
  if (!u.dynamic) {
    fprintf(stderr, "[ERROR] attempted to update a static uniform.\n");
    return u;
  }
  // update by type
  switch (u.type) {
  case INT:
    u.ival = u.iUpdate(u.ival);
    break;
  case FLOAT:
    u.fval = u.fUpdate(u.fval);
    break;
  default:
    u.fpval = u.vecUpdate(u.fpval);
    break;
  }
  return u;
}

// update all uniforms in a collection
uniform_collection update_uniforms(uniform_collection uc) {
  for (int i = 0; i < uc.count; i++) {
    uc.item[i] = update_uniform(uc.item[i]);
  }
  return uc;
}

// add a uniform to a uniform collection
void add_uniform(uniform_collection* uc, uniform u) {
  uc->count++;
  uc->item = realloc(uc->item, sizeof(uniform) * uc->count);
}

// free a collection of uniforms
void free_uniforms(uniform_collection* uc) {
  if (uc->count != -1) {
    free(uc->item);
  }
  uc->count = -1; // indicate that we've already freed this uniform
}
