#include <gtest/gtest.h>

extern "C" {
  #include "win.h"
}

#define WIDTH 800
#define HEIGHT 800

TEST(win, load_window) {
  ASSERT_EQ(1, 1);
}
