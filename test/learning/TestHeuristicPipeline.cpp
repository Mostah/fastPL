#include "../../include/app.h"
#include "../../include/learning/HeuristicPipeline.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

Config getHeuristicTestConf() {
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

Criteria getHeuristicTestCriteria() {
  std::vector<Criterion> crit_vect;
  crit_vect.push_back(Criterion("crit0", -1, 0.5));
  crit_vect.push_back(Criterion("crit1", -1, 0.2));
  crit_vect.push_back(Criterion("crit2", -1, 0.3));
  return Criteria(crit_vect);
}

Profiles getHeuristicTestProfile() {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = getHeuristicTestCriteria();
  std::vector<float> given_perf0 = {0.8, 1, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  std::vector<float> given_perf2 = {0.6, 0, 0.2};
  perf_vect.push_back(createVectorPerf("cat0", crit, given_perf2));
  perf_vect.push_back(createVectorPerf("cat1", crit, given_perf1));
  perf_vect.push_back(createVectorPerf("cat2", crit, given_perf0));
  return Profiles(perf_vect, "alt");
}

Categories getHeuristicTestCategories() { return Categories(4); }

TEST(TestHeuristicPipeline, TestComputeAccuracy) {
  Profiles profile = getHeuristicTestProfile();
  Criteria criteria = getHeuristicTestCriteria();
  Categories categories = getHeuristicTestCategories();
  MRSortModel mrsort = MRSortModel(criteria, profile, categories, 0.3);

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> alt0 = {0.9, 0.6, 0.5};
  std::vector<float> alt1 = {0.9, 0.05, 0.35};
  std::vector<float> alt2 = {0.7, 1, 0.5};
  std::vector<float> alt3 = {0.5, 0, 0.6};
  perf_vect.push_back(createVectorPerf("alt0", criteria, alt0));
  perf_vect.push_back(createVectorPerf("alt1", criteria, alt1));
  perf_vect.push_back(createVectorPerf("alt2", criteria, alt2));
  perf_vect.push_back(createVectorPerf("alt3", criteria, alt3));

  std::unordered_map<std::string, Category> truth;
  truth["alt0"] = categories.getCategoryOfRank(3);
  truth["alt1"] = categories.getCategoryOfRank(3);
  truth["alt2"] = categories.getCategoryOfRank(3);
  truth["alt3"] = categories.getCategoryOfRank(0);
  AlternativesPerformance ap = AlternativesPerformance(perf_vect, truth);

  Config conf = getHeuristicTestConf();
  HeuristicPipeline hp = HeuristicPipeline(conf, ap);
  hp.computeAccuracy(mrsort);

  EXPECT_FLOAT_EQ(mrsort.accuracy, 0.75);
}

TEST(TestHeuristicPipeline, TestOrderModels) {
  Profiles profile = getHeuristicTestProfile();
  Criteria criteria = getHeuristicTestCriteria();
  Categories categories = getHeuristicTestCategories();
  MRSortModel mrsort0 =
      MRSortModel(criteria, profile, categories, 0.4, "model0"); // acc of 1
  MRSortModel mrsort1 =
      MRSortModel(criteria, profile, categories, 0.4, "model1"); // acc of 1
  MRSortModel mrsort2 =
      MRSortModel(criteria, profile, categories, 0.3, "model2"); // acc of 0.75
  MRSortModel mrsort3 =
      MRSortModel(criteria, profile, categories, 0.3, "model3"); // acc of 0.75

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> alt0 = {0.9, 0.6, 0.5};
  std::vector<float> alt1 = {0.9, 0.05, 0.35};
  std::vector<float> alt2 = {0.7, 1, 0.5};
  std::vector<float> alt3 = {0.5, 0, 0.6};
  perf_vect.push_back(createVectorPerf("alt0", criteria, alt0));
  perf_vect.push_back(createVectorPerf("alt1", criteria, alt1));
  perf_vect.push_back(createVectorPerf("alt2", criteria, alt2));
  perf_vect.push_back(createVectorPerf("alt3", criteria, alt3));

  std::unordered_map<std::string, Category> truth;
  truth["alt0"] = categories.getCategoryOfRank(3);
  truth["alt1"] = categories.getCategoryOfRank(3);
  truth["alt2"] = categories.getCategoryOfRank(3);
  truth["alt3"] = categories.getCategoryOfRank(0);
  AlternativesPerformance ap = AlternativesPerformance(perf_vect, truth);

  std::vector<MRSortModel> models = {mrsort3, mrsort0, mrsort2, mrsort1};

  Config conf = getHeuristicTestConf();
  HeuristicPipeline hp = HeuristicPipeline(conf, ap);
  hp.orderModels(models, false);
  EXPECT_FLOAT_EQ(models[0].accuracy, 1);
  EXPECT_FLOAT_EQ(models[1].accuracy, 1);
  EXPECT_FLOAT_EQ(models[2].accuracy, 0.75);
  EXPECT_FLOAT_EQ(models[3].accuracy, 0.75);
}
