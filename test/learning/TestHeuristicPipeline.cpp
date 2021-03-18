#include "../../include/config.h"
#include "../../include/learning/HeuristicPipeline.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

Config getHeuristicTestConf() {
  Config conf;
  conf.data_dir = "../data/tests/";
  conf.model_batch_size = 50;
  conf.max_iterations = 100;
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
Categories getHeuristicTestCategories2() { return Categories(2); }

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

  EXPECT_FLOAT_EQ(mrsort.getScore(), 0.75);
}

TEST(TestHeuristicPipeline, TestCustomSort) {
  Profiles profile = getHeuristicTestProfile();
  Criteria criteria = getHeuristicTestCriteria();
  Categories categories = getHeuristicTestCategories();
  MRSortModel mrsort0 =
      MRSortModel(criteria, profile, categories, 0.4, "model0");
  MRSortModel mrsort1 =
      MRSortModel(criteria, profile, categories, 0.4, "model1");
  MRSortModel mrsort2 =
      MRSortModel(criteria, profile, categories, 0.4, "model2");
  MRSortModel mrsort3 =
      MRSortModel(criteria, profile, categories, 0.4, "model3");
  MRSortModel mrsort4 =
      MRSortModel(criteria, profile, categories, 0.4, "model4");

  mrsort0.getScore() = 1;
  mrsort1.getScore() = 0.8;
  mrsort2.getScore() = 0.6;
  mrsort3.getScore() = 0.4;
  mrsort4.getScore() = 0.2;

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> alt0 = {0.9, 0.6, 0.5};
  perf_vect.push_back(createVectorPerf("alt0", criteria, alt0));
  std::unordered_map<std::string, Category> truth;
  AlternativesPerformance ap = AlternativesPerformance(perf_vect, truth);
  Config conf = getHeuristicTestConf();

  HeuristicPipeline hp = HeuristicPipeline(conf, ap);
  hp.models.push_back(mrsort3);
  hp.models.push_back(mrsort0);
  hp.models.push_back(mrsort2);
  hp.models.push_back(mrsort1);
  hp.models.push_back(mrsort4);
  hp.customSort();
  EXPECT_EQ(hp.models[0].getId(), "model0");
  EXPECT_EQ(hp.models[1].getId(), "model1");
  EXPECT_EQ(hp.models[2].getId(), "model2");
  EXPECT_EQ(hp.models[3].getId(), "model3");
  EXPECT_EQ(hp.models[4].getId(), "model4");
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

  Config conf = getHeuristicTestConf();
  HeuristicPipeline hp = HeuristicPipeline(conf, ap);
  hp.models.push_back(mrsort3);
  hp.models.push_back(mrsort0);
  hp.models.push_back(mrsort2);
  hp.models.push_back(mrsort1);
  hp.orderModels(false);
  EXPECT_FLOAT_EQ(hp.models[0].getScore(), 1);
  EXPECT_FLOAT_EQ(hp.models[1].getScore(), 1);
  EXPECT_FLOAT_EQ(hp.models[2].getScore(), 0.75);
  EXPECT_FLOAT_EQ(hp.models[3].getScore(), 0.75);
}

TEST(TestHeuristicPipeline, TestPipeline) {
  Criteria criteria = getHeuristicTestCriteria();
  Categories categories = getHeuristicTestCategories();

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
  truth["alt0"] = categories.getCategoryOfRank(1);
  truth["alt1"] = categories.getCategoryOfRank(1);
  truth["alt2"] = categories.getCategoryOfRank(1);
  truth["alt3"] = categories.getCategoryOfRank(0);
  AlternativesPerformance ap = AlternativesPerformance(perf_vect, truth);

  Config conf = getHeuristicTestConf();

  HeuristicPipeline hp = HeuristicPipeline(conf, ap);
  hp.start();
  for (int i = 0; i < hp.models.size(); i++) {
    std::cout << hp.models[i].getScore() << std::endl;
  }
  // EXPECT_EQ(hp.models[0].getScore(), 1);
}