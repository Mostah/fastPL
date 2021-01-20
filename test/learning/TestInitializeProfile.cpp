#include "../../include/app.h"
#include "../../include/learning/InitializeProfile.h"
#include "../../include/utils.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

Config getTestConf() {
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

TEST(TestProfileInitializer, TestComputeFrequency) {
  Config conf = getTestConf();
  Criteria crit = Criteria(3, "crit");
  crit.generateRandomCriteriaWeights(0);
  PerformanceTable perf_table = PerformanceTable(4, crit, "alt");
  perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0}, {"alt1", cat1}, {"alt2", cat0}, {"alt3", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  std::vector<float> freq = profInit.categoryFrequency();
  EXPECT_EQ(freq[0], 0.5);
  EXPECT_EQ(freq[1], 0.5);
}

TEST(TestProfileInitializer, TestGetProfileCandidates) {
  Config conf = getTestConf();
  Criteria crit = Criteria(3, "crit");
  crit.generateRandomCriteriaWeights(0);
  PerformanceTable perf_table = PerformanceTable(4, crit, "alt");
  perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  Category cat2 = Category("cat2", 2);

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0}, {"alt1", cat2}, {"alt2", cat0}, {"alt3", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  std::vector<std::string> candidates =
      profInit.getProfilePerformanceCandidates(crit[0], cat0, 3);
  std::sort(candidates.begin(), candidates.end());
  EXPECT_EQ(candidates[0], "alt0");
  EXPECT_EQ(candidates[1], "alt2");
  EXPECT_EQ(candidates[2], "alt3");

  std::vector<std::string> candidates2 =
      profInit.getProfilePerformanceCandidates(crit[0], cat2, 3);
  std::ostringstream os;
  os << candidates2;
  std::sort(candidates2.begin(), candidates2.end());
  EXPECT_EQ(candidates2[0], "alt1");
  EXPECT_EQ(candidates2[1], "alt3");
}

TEST(TestProfileInitializer, TestWeightedProbability) {
  Config conf = getTestConf();
  Criteria crit = Criteria(1, "crit");
  crit.generateRandomCriteriaWeights(0);
  std::vector<Performance> perf_vect;
  for (int i = 0; i < 4; i++) {
    std::vector<float> given_perf = {static_cast<float>(0.2 * (i + 1))};
    perf_vect.push_back(
        Performance(crit, given_perf, "alt" + std::to_string(i)));
  }

  PerformanceTable perf_table = PerformanceTable(perf_vect);

  // perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  Category cat2 = Category("cat2", 2);

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0}, {"alt1", cat2}, {"alt2", cat0}, {"alt3", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  std::vector<float> freq = profInit.categoryFrequency();
  float proba =
      profInit.weightedProbability("alt0", crit[0], cat1, cat0, 3, freq);
  EXPECT_EQ(proba, 6);
}

TEST(TestProfileInitializer, TestInitializeProfilePerformance) {
  Config conf = getTestConf();
  Criteria crit = Criteria(1, "crit");
  crit.generateRandomCriteriaWeights(0);
  std::vector<Performance> perf_vect;
  for (int i = 0; i < 4; i++) {
    std::vector<float> given_perf = {static_cast<float>(0.2 * (i + 1))};
    perf_vect.push_back(
        Performance(crit, given_perf, "alt" + std::to_string(i)));
  }

  PerformanceTable perf_table = PerformanceTable(perf_vect);

  Categories categories = Categories(3);

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"alt0", categories[0]},
                                                {"alt1", categories[1]},
                                                {"alt2", categories[2]},
                                                {"alt3", categories[1]}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  const std::vector<float> freq = profInit.categoryFrequency();
  std::vector<float> profileCrit =
      profInit.initializeProfilePerformance(crit[0], categories, freq);
  std::ostringstream os;
  os << profileCrit;
  EXPECT_EQ(profileCrit.size(), categories.getNumberCategories() - 1);
  EXPECT_EQ(os.str(), "[0.8,0.6]");
}