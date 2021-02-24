#include "../../include/types/Criteria.h"
#include "../../include/types/MRSortModel.h"
#include "../../include/types/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

//        crit0  crit1  crit2
// cat0
//         0.8     1     0.4
// cat1
//         0.8    0.1    0.3
// cat2
//         0.6     0     0.2
// base

Criteria getTestCriteria() {
  std::vector<Criterion> crit_vect;
  crit_vect.push_back(Criterion("crit0", -1, 0.5));
  crit_vect.push_back(Criterion("crit1", -1, 0.2));
  crit_vect.push_back(Criterion("crit2", -1, 0.3));
  return Criteria(crit_vect);
}

Profiles getTestProfile() {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = getTestCriteria();
  std::vector<float> given_perf0 = {0.8, 1, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  std::vector<float> given_perf2 = {0.6, 0, 0.2};
  perf_vect.push_back(createVectorPerf("cat0", crit, given_perf2));
  perf_vect.push_back(createVectorPerf("cat1", crit, given_perf1));
  perf_vect.push_back(createVectorPerf("cat2", crit, given_perf0));
  return Profiles(perf_vect, "alt");
}

Categories getTestCategories() { return Categories(4); }

TEST(TestMRSortModel, TestBaseConstructorError) {
  Criteria crit = Criteria(1);
  Profiles prof = Profiles(1, crit, "crit", "prof");

  Categories cats = Categories(1);
  try {
    MRSortModel mrsort = MRSortModel(crit, prof, cats, 0.5);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("For n categories we must have n - 1 profiles."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestMRSortModel, TestCategoryAssignment) {
  Profiles profile = getTestProfile();
  Criteria criteria = getTestCriteria();
  Categories categories = getTestCategories();
  MRSortModel mrsort = MRSortModel(criteria, profile, categories, 0.6);

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> alt0 = {0.9, 0.6, 0.5};
  std::vector<float> alt1 = {0.9, 0.05, 0.35};
  std::vector<float> alt2 = {0.7, 1, 0.5};
  std::vector<float> alt3 = {0.5, 0, 0.6};
  perf_vect.push_back(createVectorPerf("alt0", criteria, alt0));
  perf_vect.push_back(createVectorPerf("alt1", criteria, alt1));
  perf_vect.push_back(createVectorPerf("alt2", criteria, alt2));
  perf_vect.push_back(createVectorPerf("alt3", criteria, alt3));
  PerformanceTable pt_ = PerformanceTable(perf_vect);

  std::unordered_map<std::string, Category> expected_assignment;
  expected_assignment["alt0"] = categories.getCategoryOfRank(3);
  expected_assignment["alt1"] = categories.getCategoryOfRank(3);
  expected_assignment["alt2"] = categories.getCategoryOfRank(3);
  expected_assignment["alt3"] = categories.getCategoryOfRank(0);

  AlternativesPerformance ap = mrsort.categoryAssignment(pt_);
  EXPECT_EQ(expected_assignment, ap.getAlternativesAssignments());
}