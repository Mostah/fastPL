#include "../../include/app.h"
#include "../../include/learning/WeightUpdater.h"
#include "../../include/types/AlternativesPerformance.h"
#include "../../include/types/Categories.h"
#include "../../include/types/Category.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/MRSortModel.h"
#include "../../include/types/Perf.h"
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
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crits = Criteria(2, "crit");
  std::vector<float> vec0 = {0.8, 0.6}; // good candidate
  std::vector<float> vec1 = {1, 0.5};   // good candidate
  std::vector<float> vec2 = {0.2, 0.4}; // bad candidate
  perf_vect.push_back(createVectorPerf("a0", crits, vec0));
  perf_vect.push_back(createVectorPerf("a1", crits, vec1));
  perf_vect.push_back(createVectorPerf("a2", crits, vec2));
  Category cat0 = Category("cat0", 0); // bad category
  Category cat1 = Category("cat1", 1); // good category

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"a0", cat1}, {"a1", cat1}, {"a2", cat0}};
  AlternativesPerformance ap = AlternativesPerformance(perf_vect, map);
  return ap;
}

MRSortModel getModelTest() {
  auto ap = getAPTest();

  std::vector<std::string> cats_vect = {"cat0", "cat1", "cat2"};

  Categories cats = Categories(cats_vect);

  Criteria crits = Criteria(2, "crit");

  std::vector<float> vec0 = {0.3, 0.4};
  std::vector<float> vec1 = {0.7, 0.5};
  std::vector<std::vector<Perf>> perf_vect;
  perf_vect.push_back(createVectorPerf("cat0", crits, vec0));
  perf_vect.push_back(createVectorPerf("cat1", crits, vec1));
  Profiles profs = Profiles(perf_vect, "alt");

  MRSortModel model = MRSortModel(crits, profs, cats, 0.6);
  return model;
}

TEST(TestWeightUpdater, TestComputeXMatrix) {
  Config conf = getWeightTestConf();
  auto ap = getAPTest();
  auto model = getModelTest();
  WeightUpdater wu = WeightUpdater(ap, conf);
  auto x_matrix = wu.computeXMatrix(model);
  std::vector<std::vector<std::vector<bool>>> x_matrix_expected = {
      {{true, true}, {true, true}, {}}};
  EXPECT_EQ(x_matrix, x_matrix_expected);
}

TEST(TestWeightUpdater, TestComputeYMatrix) {
  Config conf = getWeightTestConf();
  auto ap = getAPTest();
  auto model = getModelTest();
  WeightUpdater wu = WeightUpdater(ap, conf);

  auto y_matrix = wu.computeYMatrix(model);
  std::vector<std::vector<std::vector<bool>>> y_matrix_expected = {
      {{}, {}, {false, true}}};
  EXPECT_EQ(y_matrix, y_matrix_expected);
}

TEST(TestWeightUpdater, TestCompleteSolve) {
  Config conf = getWeightTestConf();
  auto ap = getAPTest();
  auto model = getModelTest();
  WeightUpdater wu = WeightUpdater(ap, conf);
  wu.updateWeightsAndLambda(model);

  // results for y_a2 = 1, the rest is 0
  EXPECT_EQ(1, model.lambda);
  EXPECT_EQ(1, model.criteria[0].getWeight());
  EXPECT_EQ(0, model.criteria[1].getWeight());
}