//============================//
//                            //
//        uniformbuf.c        //
//                            //
//============================================//
// handles adding and removing to uniformbuf. //
//============================================================================80

#include "uniformbuf.h"

// make a new buffer
UniformBuf new_uniformbuf() {
  UniformBuf ubuf = {.buf = malloc(0), .count = 0};
  return ubuf;
}

void free_uniformbuf(UniformBuf ubuf) { free(ubuf.buf); }

// expand buffer, add uniform, increment count
void uniformbuf_add(UniformBuf *ubuf, Uniform u) {
  ubuf->buf = realloc(ubuf->buf, ubuf->count * sizeof(Uniform));
  ubuf->buf[ubuf->count] = u;
  ubuf->count++;
}

// remove an index from the buffer
void uniformbuf_remove(UniformBuf *ubuf, int index) {
  // shift data
  for (int i = index; i < ubuf->count - 1; i++) {
    ubuf->buf[i] = ubuf->buf[i + 1];
  }
  // lop off the end
  ubuf->count--;
  ubuf->buf = realloc(ubuf->buf, ubuf->count * sizeof(Uniform));
}

// print a uniform buffer for debugging
void uniformbuf_print(UniformBuf ubuf) {
  printf("UniformBuf: \n");
  printf("  - count: %u\n", (ubuf.count));
}

void uniformbuf_update(UniformBuf* ubuf) {
  for (int i = 0; i < ubuf->count; i++) {
    if (ubuf->buf[i].dynamic) {
      update_uniform(&(ubuf->buf[i]));
    }
  }
}
