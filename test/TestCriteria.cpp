#include "../src/Criteria.h"
#include "../src/Criterion.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestCriteria, TestBaseConstructorWithNbAndId) {
  std::string id = "test";
  Criteria crits = Criteria(2, id);
  std::ostringstream os;
  os << crits;
  EXPECT_EQ(os.str(), "Criteria(Criterion(id : test0, "
                      "direction : +, weight : 0), Criterion(id : "
                      "test1, direction : +, weight : 0), )");
}

TEST(TestCriteria, TestCriterionVectConstructor) {
  std::string id = "test";
  int direction = -1;
  float weight = 0.4;
  Criterion crit = Criterion(id, direction, weight);

  std::vector<Criterion> crit_vect;
  crit_vect.push_back(crit);

  Criteria crits = Criteria(crit_vect);

  std::ostringstream os;
  os << crits;
  EXPECT_EQ(os.str(), "Criteria(Criterion(id : test, "
                      "direction : -, weight : 0.4), )");
}

TEST(TestCriteria, TestCopyConstructor) {
  std::string id = "test";
  int direction = -1;
  float weight = 0.4;
  Criterion crit = Criterion(id, direction, weight);

  std::vector<Criterion> crit_vect;
  crit_vect.push_back(crit);

  Criteria crits = Criteria(crit_vect);

  Criteria crits_copied = Criteria(crits);

  std::ostringstream os;
  os << crits_copied;
  EXPECT_EQ(os.str(), "Criteria(Criterion(id : test, "
                      "direction : -, weight : 0.4), )");
}

TEST(TestCriteria, TestSetCriterionVect) {
  std::string id = "test";
  int direction = -1;
  float weight = 0.4;
  Criterion crit = Criterion(id, direction, weight);

  std::vector<Criterion> crit_vect;
  crit_vect.push_back(crit);

  Criteria crits = Criteria(2, id);
  crits.setCriterionVect(crit_vect);

  std::ostringstream os;
  os << crits;
  EXPECT_EQ(os.str(), "Criteria(Criterion(id : test, "
                      "direction : -, weight : 0.4), )");
}

TEST(TestCriteria, TestGetMaxWeight) {
  std::vector<Criterion> crit_vect0;
  Criteria crits = Criteria(crit_vect0);

  float max_weight = crits.getMaxWeight();
  EXPECT_FLOAT_EQ(0, max_weight);

  std::vector<Criterion> crit_vect1;
  crit_vect1.push_back(Criterion("test0", -1, 0.4));
  crit_vect1.push_back(Criterion("test1", -1, 0.6));
  crits.setCriterionVect(crit_vect1);

  max_weight = crits.getMaxWeight();
  EXPECT_FLOAT_EQ(0.6, max_weight);
}

TEST(TestCriteria, TestGetMinWeight) {
  std::vector<Criterion> crit_vect0;
  Criteria crits = Criteria(crit_vect0);

  float min_weight = crits.getMinWeight();
  EXPECT_FLOAT_EQ(0, min_weight);

  std::vector<Criterion> crit_vect1;
  crit_vect1.push_back(Criterion("test0", -1, 0.4));
  crit_vect1.push_back(Criterion("test1", -1, 0.6));
  crits.setCriterionVect(crit_vect1);

  min_weight = crits.getMinWeight();
  EXPECT_FLOAT_EQ(0.4, min_weight);
}

TEST(TestCriteria, TestGetSumWeight) {
  std::vector<Criterion> crit_vect0;
  Criteria crits = Criteria(crit_vect0);

  float sum_weight = crits.getSumWeight();
  EXPECT_FLOAT_EQ(0, sum_weight);

  std::vector<Criterion> crit_vect1;
  crit_vect1.push_back(Criterion("test0", -1, 0.4));
  crit_vect1.push_back(Criterion("test1", -1, 0.6));
  crits.setCriterionVect(crit_vect1);

  sum_weight = crits.getSumWeight();
  EXPECT_FLOAT_EQ(1, sum_weight);
}

TEST(TestCriteria, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}