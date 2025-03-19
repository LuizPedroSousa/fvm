#include <gtest/gtest.h>

#define REPEAT 1

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

  for (int i = 0; i < REPEAT; i++) {
    int result = RUN_ALL_TESTS();

    if (result != 0) {
      return result;
    }
  }

  return 0;
}
