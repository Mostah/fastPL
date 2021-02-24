#include "../../include/types/ProfileUpdater.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

//               THESIS CASE

//        crit0  crit1  crit2  crit3  crit4
// ----------------------------------------
//                   cat2
// b1------0.6----0.6----0.6----0.6----0.6
//                   cat1                         w=0.2  lambda=0.8  delta=0.1
// b0------0.3----0.3----0.3----0.3----0.3
//                   cat0
// ----------------------------------------

Criteria newTestCriteria() {
  std::vector<Criterion> crit_vect;
  crit_vect.push_back(Criterion("crit0", 1, 0.2));
  crit_vect.push_back(Criterion("crit1", -1, 0.2));
  crit_vect.push_back(Criterion("crit2", 1, 0.2));
  crit_vect.push_back(Criterion("crit3", 1, 0.2));
  crit_vect.push_back(Criterion("crit4", -1, 0.2));
  return Criteria(crit_vect);
}

Profiles newTestProfile() {
  std::vector<Performance> perf_vect;
  Criteria crit = newTestCriteria();
  std::vector<float> given_perf0 = {0.3, 0.3, 0.3, 0.3, 0.3};
  std::vector<float> given_perf1 = {0.6, 0.6, 0.6, 0.6, 0.6};
  perf_vect.push_back(Performance(crit, given_perf1, "b1"));
  perf_vect.push_back(Performance(crit, given_perf0, "b0"));
  return Profiles(perf_vect);
}

Categories newTestCategories() {
  Categories cat = Categories(3);
  return cat;
}

MRSortModel newTestModel(Categories &categories) {
  Profiles profile = newTestProfile();
  Criteria criteria = newTestCriteria();
  MRSortModel model = MRSortModel(criteria, profile, categories, 0.8);
  return model;
}

AlternativesPerformance newTestAltPerf() {
  Criteria criteria = newTestCriteria();
  Categories categories = newTestCategories();
  std::vector<Performance> perf_vect;
  // ** Alt0 ** data: cat0 - model: cat0
  std::vector<float> alt0 = {0.5, 0.35, 0.45, 0.1, 0.21};
  // ** Alt1 ** data: cat0 - model: cat1
  std::vector<float> alt1 = {0.38, 0.5, 0.56, 0.58, 0.55};
  // ** Alt2 ** data: cat1 - model: cat0
  std::vector<float> alt2 = {0.4, 0.24, 0.2, 0.39, 0.75};
  // ** Alt3 ** data: cat0 - model: cat2
  std::vector<float> alt3 = {0.75, 0.75, 0.67, 0.55, 0.9};
  perf_vect.push_back(Performance(criteria, alt0, "alt0"));
  perf_vect.push_back(Performance(criteria, alt1, "alt1"));
  perf_vect.push_back(Performance(criteria, alt2, "alt2"));
  perf_vect.push_back(Performance(criteria, alt3, "alt3"));

  std::unordered_map<std::string, Category> assignment;
  assignment["alt0"] = categories.getCategoryOfRank(0);
  assignment["alt1"] = categories.getCategoryOfRank(0);
  assignment["alt2"] = categories.getCategoryOfRank(1);
  assignment["alt3"] = categories.getCategoryOfRank(0);
  return AlternativesPerformance(perf_vect, assignment);
}

TEST(TestProfileUpdater, TestBaseConstructor) {
  // MRSortModel model = newTestModel();
  AlternativesPerformance altPerf_data = newTestAltPerf();
  ProfileUpdater profUpdater = ProfileUpdater(altPerf_data, 0.1);

  EXPECT_EQ("hello", "hello");
}

TEST(TestProfileUpdater, TestComputeAboveDesirability) {
  Categories categories = newTestCategories();
  MRSortModel model = newTestModel(categories);
  AlternativesPerformance altPerf_data = newTestAltPerf();
  std::unordered_map<std::string, std::unordered_map<std::string, float>> ct =
      model.computeConcordanceTable(altPerf_data);
  std::unordered_map<std::string, float> ct_b0 = ct["b0"];
  float delta = 0.09;
  ProfileUpdater profUpdater = ProfileUpdater(altPerf_data, delta);
  Perf b0 = Perf("b0", "crit0", 0.3);
  Perf b1 = Perf("b1", "crit0", 0.6);
  Category cat = categories.getCategoryOfRank(0);
  Category cat_above = categories.getCategoryOfRank(1);

  std::unordered_map<float, float> above_des =
      profUpdater.computeAboveDesirability(model, "crit0", b0, b1, cat,
                                           cat_above, ct_b0);
  EXPECT_FLOAT_EQ(
      above_des[altPerf_data.getPerf("alt1", "crit0").getValue() + delta], 0.5);
  EXPECT_EQ("hello", "hello");
}