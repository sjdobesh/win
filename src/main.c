/*===========================*
 *                           *
 *    main driver program    *
 *                           *
 *======================================*
 * samantha jane                        *
 * driver program for window rendering. *
 *==========================================================================80*/

#include "win.h"

int inc(int x) {
  return x++;
}

int main() {

  win w = {
    .w = 1000,
    .h = 1000,
    .prog = {
      .frag = {
        .path = "default.frag"
      }
    }
  };
  
  init_win(&w);
  while(1)
    render_win(w);
}
