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

TEST(TestCriterion, TestGetterSetter) {
  std::string id = "test";
  std::string name = "criterion_test";
  int direction = -1;
  float weight = 0.4;
  Criterion crit = Criterion(id, name, direction, weight);
  EXPECT_EQ("test", crit.getId());
  EXPECT_EQ("criterion_test", crit.getName());
  EXPECT_EQ(-1, crit.getDirection());
  EXPECT_FLOAT_EQ(0.4, crit.getWeight());

  crit.setId("test_set");
  crit.setName("criterion_test_set");
  crit.setDirection(1);
  crit.setWeight(0.3);

  EXPECT_EQ("test_set", crit.getId());
  EXPECT_EQ("criterion_test_set", crit.getName());
  EXPECT_EQ(1, crit.getDirection());
  EXPECT_FLOAT_EQ(0.3, crit.getWeight());
}

TEST(TestCriterion, TestDirectionGenerator) {
  std::string id = "test_direction";
  Criterion crit = Criterion(id);
  unsigned long int seed = time(0);

  // generating a direction to compare to the one generated in generateDirection
  // should be the same since we are using the same seed
  srand(seed);
  int direction = 1;
  if (((float)rand() / RAND_MAX) < 0.5) {
    direction = -1;
  }
  crit.generateDirection(seed);

  EXPECT_FLOAT_EQ(direction, crit.getDirection());
}

TEST(TestCriterion, TestWeightGenerator) {
  std::string id = "test_weight";
  Criterion crit = Criterion(id);
  unsigned long int seed = time(0);

  // generating a weight to compare to the one generated in generateWeight
  // should be the same since we are using the same seed
  srand(seed);
  float weight = ((float)rand() / RAND_MAX);
  crit.generateWeight(seed);

  EXPECT_FLOAT_EQ(weight, crit.getWeight());
}

TEST(TestCriterion, TestAllCriterionInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}
