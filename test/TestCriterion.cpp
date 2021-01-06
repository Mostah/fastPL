#include "../include/Criterion.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestCriterion, TestBaseConstructorWithoutSpec) {
  std::string id = "test_no_spec";
  Criterion crit = Criterion(id);
  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(),
            "Criterion(id : test_no_spec, name : , direction : +, weight : 0)");
}

TEST(TestCriterion, TestBaseConstructor) {
  std::string id = "test";
  std::string name = "criterion_test";
  int direction = -1;
  float weight = 0.4;
  Criterion crit = Criterion(id, name, direction, weight);
  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(), "Criterion(id : test, name : criterion_test, direction : "
                      "-, weight : 0.4)");
}

TEST(TestCriterion, TestConstructorByCopy) {
  std::string id = "test_copy";
  std::string name = "criterion_by_copy";
  int direction = 1;
  float weight = 0.3;
  Criterion crit = Criterion(id, name, direction, weight);

  Criterion critCopied = Criterion(crit);

  std::ostringstream os;
  os << critCopied;
  EXPECT_EQ(os.str(), "Criterion(id : test_copy, name : criterion_by_copy, "
                      "direction : +, weight : 0.3)");
}

TEST(TestCriterion, TestAllCriterionInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}
