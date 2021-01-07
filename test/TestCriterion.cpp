#include "../src/Criterion.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestCriterion, TestBaseConstructorWithoutSpec) {
  std::string id = "test_no_spec";
  Criterion crit = Criterion(id);
  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(),
            "Criterion(id : test_no_spec, direction : +, weight : 0)");
}

TEST(TestCriterion, TestBaseConstructor) {
  std::string id = "test";
  int direction = -1;
  float weight = 0.4;
  Criterion crit = Criterion(id, direction, weight);
  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(), "Criterion(id : test, direction : -, weight : 0.4)");
}

TEST(TestCriterion, TestConstructorByCopy) {
  std::string id = "test_copy";
  int direction = 1;
  float weight = 0.3;
  Criterion crit = Criterion(id, direction, weight);

  Criterion critCopied = Criterion(crit);

  std::ostringstream os;
  os << critCopied;
  EXPECT_EQ(os.str(), "Criterion(id : test_copy, direction : +, weight : 0.3)");
}

TEST(TestCriterion, TestRandomWeightGeneration) {
  std::string id = "crit1";
  Criterion crit = Criterion(id);
  crit.getRandomCriterionWeight(0);
  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(), "Criterion(id : crit1, direction : +, weight : 0.383)");
}

TEST(TestCriterion, TestAllCriterionInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}
