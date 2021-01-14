#include "../include/utils.h"
#include "gtest/gtest.h"
#include <iostream>
#include <ostream>
#include <utility>

TEST(TestUtils, TestRandomIntGenerator) {
  int random1 = getRandomUniformInt(0, 0, 20);
  int random2 = getRandomUniformInt(0, 0, 20);
  int random3 = getRandomUniformInt(40, 0, 20);

  EXPECT_EQ(random1, random2);
  EXPECT_NE(random1, random3);
}

TEST(TestUtils, TestRandomFloatGenerator) {
  float random1 = getRandomUniformFloat(0, 0, 1);
  float random2 = getRandomUniformFloat(0, 0, 1);
  float random3 = getRandomUniformInt(40, 0, 1);

  EXPECT_EQ(random1, random2);
  EXPECT_NE(random1, random3);
}