#include "../include/utils.h"
#include "gtest/gtest.h"
#include <iostream>
#include <ostream>
#include <utility>

TEST(TestUtils, TestRandomIntGenerator) {
  int random1 = getRandomUniformInt(0, 20, 0);
  int random2 = getRandomUniformInt(0, 20, 0);
  int random3 = getRandomUniformInt(0, 20, 40);

  EXPECT_EQ(random1, random2);
  EXPECT_NE(random1, random3);
}

TEST(TestUtils, TestRandomFloatGenerator) {
  float random1 = getRandomUniformFloat(1, 4, 0);
  float random2 = getRandomUniformFloat(1, 4, 0);
  float random3 = getRandomUniformFloat(1, 4, 40);

  float random4 = getRandomUniformFloat();

  EXPECT_EQ(random1, random2);
  EXPECT_NE(random1, random3);
  EXPECT_TRUE(random4 >= 0 && random4 <= 1);
}