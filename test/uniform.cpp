#include <gtest/gtest.h>

extern "C" {
  #include "uniform.h"
}

#define WIDTH 800
#define HEIGHT 800

TEST(uniform, init) {
  ASSERT_EQ(1, 1);
}
