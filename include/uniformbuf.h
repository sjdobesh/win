//============================//
//                            //
//        uniformbuf.h        //
//                            //
//==========================================//
// buffer of all uniform data for a shader. //
//============================================================================80

#ifndef _UNIFORMBUF_H_
#define _UNIFORMBUF_H_

#include "uniform.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct UniformBuf {
  Uniform *buf;
  unsigned int count;
} UniformBuf;

void uniformbuf_update(UniformBuf* ubuf);
#endif
