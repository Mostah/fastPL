#include "../../include/types/Criteria.h"
#include "../../include/types/Criterion.h"
#include "../../include/utils.h"
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

  std::vector<Criterion> crit_vect_err;
  crit_vect_err.push_back(Criterion("a", -1, 0.4));
  crit_vect_err.push_back(Criterion("a", -1, 0));

  try {
    Criteria crits_err = Criteria(crit_vect_err);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Each criterion must have different ids."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestCriteria, TestBaseConstructorWithNbOnly) {
  Criteria criteria1 = Criteria(2);
  std::ostringstream os;
  os << criteria1;
  EXPECT_EQ(os.str(),
            "Criteria(Criterion(id : crit0, direction : +, weight : 0), "
            "Criterion(id : crit1, direction : +, weight : 0), )");
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

TEST(TestCriteria, TestOperatorSubscript) {
  Criteria criteria1 = Criteria(1);
  Criterion crit = criteria1[0];

  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(), "Criterion(id : crit0, direction : +, weight : 0)");
}

TEST(TestCriteria, TestOperatorConstantSubscript) {
  const Criteria criteria1 = Criteria(1);
  Criterion crit = criteria1[0];

  std::ostringstream os;
  os << crit;
  EXPECT_EQ(os.str(), "Criterion(id : crit0, direction : +, weight : 0)");
}

TEST(TestCriteria, TestGetWeights) {
  const Criteria criteria1 = Criteria(2);
  std::vector<float> w = criteria1.getWeights();
  std::ostringstream os;
  os << w;
  EXPECT_EQ(os.str(), "[0,0]");
}

TEST(TestCriteria, TestSetWeights) {
  Criteria criteria1 = Criteria(2);
  std::vector<float> w{1.1, 1.2};
  std::ostringstream os;
  criteria1.setWeights(w);
  std::vector<float> w2 = criteria1.getWeights();
  os << w2;
  EXPECT_EQ(os.str(), "[1.1,1.2]");
}

TEST(TestCriteria, TestNormalizeWeights) {
  Criteria criteria1 = Criteria(4);
  std::vector<float> w{2.2, 2.2, 2.2, 2.2};
  criteria1.setWeights(w);
  criteria1.normalizeWeights();
  std::vector<float> w2 = criteria1.getWeights();
  std::ostringstream os;
  os << w2;
  EXPECT_EQ(os.str(), "[0.25,0.25,0.25,0.25]");
}

TEST(TestCriteria, TestRandomWeights) {
  Criteria criteria1 = Criteria(3);
  criteria1.generateRandomCriteriaWeights(0);
  std::vector<float> w = criteria1.getWeights();
  std::ostringstream os;
  os << w;
  EXPECT_EQ(os.str(), "[0.333333,0.333333,0.333333]");
}