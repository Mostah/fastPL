#include "../src/Criteria.h"
#include "../src/MRSortModel.h"
#include "../src/PerformanceTable.h"
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

PerformanceTable getTestProfile() {
  std::vector<Performance> perf_vect;
  Criteria crit = getTestCriteria();
  std::vector<float> given_perf0 = {0.8, 1, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  std::vector<float> given_perf2 = {0.6, 0, 0.2};
  perf_vect.push_back(Performance(crit, given_perf0, "cat0"));
  perf_vect.push_back(Performance(crit, given_perf1, "cat1"));
  perf_vect.push_back(Performance(crit, given_perf2, "cat2"));
  return PerformanceTable(perf_vect);
}

TEST(TestMRSortModel, TestBaseConstructor) {
  Criteria crit = Criteria(1);
  PerformanceTable prof = PerformanceTable(1, crit, "prof");

  MRSortModel mrsort = MRSortModel(crit, prof, 0.5);
  std::ostringstream os;
  os << mrsort;
  EXPECT_EQ(os.str(),
            "Model( id : model, lambda : 0.5, crit : Criteria(Criterion(id "
            ": crit0, direction : +, weight : 0), ), "
            "profiles : PerformanceTable(Performance(Perf( name : "
            "prof0, crit : crit0, value : 0 ), ), ))");
}

TEST(TestMRSortModel, TestCategoryAssignment) {
  PerformanceTable profile = getTestProfile();
  Criteria criteria = getTestCriteria();
  MRSortModel mrsort = MRSortModel(criteria, profile, 0.6);

  std::vector<Performance> perf_vect;
  std::vector<float> alt0 = {0.9, 0.6, 0.5};
  std::vector<float> alt1 = {0.9, 0.05, 0.35};
  std::vector<float> alt2 = {0.7, 1, 0.5};
  std::vector<float> alt3 = {0.5, 0, 0.6};
  perf_vect.push_back(Performance(criteria, alt0, "alt0"));
  perf_vect.push_back(Performance(criteria, alt1, "alt1"));
  perf_vect.push_back(Performance(criteria, alt2, "alt2"));
  perf_vect.push_back(Performance(criteria, alt3, "alt3"));
  PerformanceTable pt_ = PerformanceTable(perf_vect);

  std::vector<std::pair<std::string, std::string>> expected_assignment;
  expected_assignment.push_back(std::make_pair("alt0", "cat0"));
  expected_assignment.push_back(std::make_pair("alt1", "cat1"));
  expected_assignment.push_back(std::make_pair("alt2", "cat2"));
  expected_assignment.push_back(std::make_pair("alt3", "base"));

  EXPECT_EQ(expected_assignment, mrsort.categoryAssignment(pt_));
}

TEST(TestMRSort, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}