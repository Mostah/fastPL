#include "../src/CriterionValue.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

#include <string>

int AtomicMCDAObject::nb_instances_{0};

TEST(TestCriterionValue, TestBaseConstructor) {
  std::string id = "test";
  float weight = 0.3;
  CriterionValue cv = CriterionValue(id, weight);

  std::ostringstream os;
  os << cv;
  EXPECT_EQ(os.str(), "CriterionValue(test: 0.3)");
}

TEST(TestCriterionValue, TestBaseConstructorWithoutWeight) {
  std::string id = "test";
  CriterionValue cv = CriterionValue(id);

  std::ostringstream os;
  os << cv;
  EXPECT_EQ(os.str(), "CriterionValue(test: 0)");
}

TEST(TestCriterionValue, TestConstructorByCopy) {
  std::string id = "test";
  float weight = 0.3;
  CriterionValue cv = CriterionValue(id, weight);

  CriterionValue cvCopied = CriterionValue(cv);

  std::ostringstream os;
  os << cvCopied;
  EXPECT_EQ(os.str(), "CriterionValue(test: 0.3)");
}

TEST(TestCriterionValue, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}