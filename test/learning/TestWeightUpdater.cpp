#include "../../include/app.h"
#include "../../include/learning/WeightUpdater.h"
#include "../../include/types/AlternativesPerformance.h"
#include "../../include/types/Category.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/MRSortModel.h"
#include "../../include/types/Performance.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

Config getWeightTestConf() {
  Config conf;
  conf.data_dir = "../data/tests/";
  try {
    conf.logger =
        spdlog::basic_logger_mt("test_logger", "../logs/test_logger.txt");
  } catch (const spdlog::spdlog_ex &ex) {
    conf.logger = spdlog::get("test_logger");
  }
  spdlog::set_level(spdlog::level::debug);
  return conf;
}

AlternativesPerformance getAPTest() {
  std::vector<Performance> perf_vect;
  Criteria crits = Criteria(2, "crit");
  std::vector<float> vec0 = {0.8, 0.6}; // good candidate
  std::vector<float> vec1 = {1, 0.5};   // good candidate
  std::vector<float> vec2 = {0.2, 0.4}; // bad candidate
  perf_vect.push_back(Performance(crits, vec0, "a0"));
  perf_vect.push_back(Performance(crits, vec1, "a1"));
  perf_vect.push_back(Performance(crits, vec2, "a2"));
  Category cat0 = Category("cat0", 0); // bad category
  Category cat1 = Category("cat1", 1); // good category

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"a0", cat1}, {"a1", cat1}, {"a2", cat0}};
  AlternativesPerformance ap = AlternativesPerformance(perf_vect, map);
  return ap;
}

Model getModelTest() {
  auto ap = getAPTest();
  Category cat0 = Category("cat0", 0); // bad category
  Category cat1 = Category("cat1", 1); // good category
  std::vector<Category> cats_vect = {cat0, cat1};
  Categories cats = Categories(cats_vect);

  Criteria crits = Criteria(2, "crit");

  std::vector<float> vec0 = {0.3, 0.4};
  std::vector<float> vec1 = {0.7, 0.5};
  std::vector<Performance> perf_vect;
  perf_vect.push_back(Performance(crits, vec0, "prof0"));
  perf_vect.push_back(Performance(crits, vec1, "prof1"));
  Profiles profs = Profiles(erf_vect);

  MRSortModel model = MRSortModel(crits, profs, cats, 0.6);
}

TEST(TestWeightUpdater, TestComputeXMatrix) {
  Config conf = getWeightTestConf;
  auto ap = getAPTest();
  auto model = getModelTest();
}

TEST(TestWeightUpdater, TestComputeYMatrix) {}