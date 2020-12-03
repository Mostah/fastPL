#include "../src/example.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestNombre, TestNombreVide) {
  Nombre n;
  std::ostringstream os;
  os << n;
  EXPECT_EQ(os.str(), "0");
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}