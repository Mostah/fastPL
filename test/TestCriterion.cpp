#include "../src/example.h"
#include "../src/Criterion.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>


TEST(TestCriterion, TestCriterionVide) {
  Criterion crit = Criterion("2");
  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(), "Criterion(2:+)");
}