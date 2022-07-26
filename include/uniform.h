//=================//
//                 //
//    uniform.h    //
//                 //
//===================================//
// handles uniform data and updates. //
//============================================================================80

#ifndef _UNIFORM_H_
#define _UNIFORM_H_

// indicate type in the union
enum uniform_type { INT, FLOAT, V2, V3, V4 };

// update function pointers
typedef int (*int_update_func)(int);
typedef float (*float_update_func)(float);
typedef float *(*vec_update_func)(float *);

// data structure for the setting and updating of uniform shader variables
typedef struct uniform {
  char *name;
  union {
    int ival;
    float fval; float *fpval;
  };
  unsigned int type;
  int dynamic;
  union {
    int_update_func iUpdate;
    float_update_func fUpdate;
    vec_update_func vecUpdate;
  };
} uniform;

typedef struct uniform_collection {
  uniform* item;
  int count;
} uniform_collection;

// prototypes

uniform new_i_uniform(char *name, int val);
uniform new_di_uniform(char *name, int val, int (*int_update_func)(int));
uniform new_f_uniform(char *name, float val);
uniform new_df_uniform(char *name, float val,
                       float (*float_update_func)(float));
uniform new_v2_uniform(char *name, float x, float y);
uniform new_dv2_uniform(char *name, float x, float y,
                        float *(*vecUpdateFunc)(float *));
uniform new_v3_uniform(char *name, float x, float y, float z);
uniform new_dv3_uniform(char *name, float x, float y, float z,
                        float *(*vecUpdateFunc)(float *));
uniform new_v4_uniform(char *name, float x, float y, float z, float w);
uniform new_dv4_uniform(char *name, float x, float y, float z, float w,
                        float *(*vecUpdateFunc)(float *));
uniform update_uniform(uniform u);
uniform_collection update_uniforms(uniform_collection uc);
void add_uniform(uniform_collection *uc, uniform u);
void free_uniforms(uniform_collection *uc);

#endif
