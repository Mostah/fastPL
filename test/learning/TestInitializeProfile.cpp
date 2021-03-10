#include "../../include/app.h"
#include "../../include/learning/ProfileInitializer.h"
#include "../../include/types/MRSortModel.h"
#include "../../include/types/Perf.h"
#include "../../include/types/Profiles.h"
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>
#include <vector>

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
  PerformanceTable perf_table = PerformanceTable(5, crit, "alt");
  perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  Category cat2 = Category("cat2", 2);

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"alt0", cat0},
                                                {"alt1", cat1},
                                                {"alt2", cat0},
                                                {"alt3", cat1},
                                                {"alt4", cat2}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  std::vector<float> freq = profInit.categoryFrequency();
  std::ostringstream os;
  os << freq;
  EXPECT_EQ(os.str(), "[0.4,0.4,0.2]");
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
  std::vector<Perf> candidates =
      profInit.getProfilePerformanceCandidates(crit[0], cat0, 3);
  std::vector<std::string> altId;
  for (auto p : candidates)
    altId.push_back(p.getName());
  std::sort(altId.begin(), altId.end());
  EXPECT_EQ(candidates[0].getName(), "alt0");
  EXPECT_EQ(candidates[1].getName(), "alt2");
  EXPECT_EQ(candidates[2].getName(), "alt3");

  std::vector<std::string> altId2;
  std::vector<Perf> candidates2 =
      profInit.getProfilePerformanceCandidates(crit[0], cat2, 3);
  for (auto p : candidates2)
    altId2.push_back(p.getName());
  std::sort(altId2.begin(), altId2.end());
  EXPECT_EQ(candidates2[0].getName(), "alt1");
  EXPECT_EQ(candidates2[1].getName(), "alt3");
}

TEST(TestProfileInitializer, TestWeightedProbability) {
  Config conf = getTestConf();
  Criteria crit = Criteria(1, "crit");
  crit.generateRandomCriteriaWeights(0);
  std::vector<std::vector<Perf>> perf_vect;
  for (int i = 0; i < 4; i++) {
    std::vector<float> given_perf = {static_cast<float>(0.2 * (i + 1))};
    perf_vect.push_back(
        createVectorPerf("alt" + std::to_string(i), crit, given_perf));
  }

  PerformanceTable perf_table = PerformanceTable(perf_vect);

  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  Category cat2 = Category("cat2", 2);

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0},
          {"alt1", cat1},
          {"alt2", cat0},
          {"alt3", cat2},
      };
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  std::vector<float> freq = profInit.categoryFrequency();
  std::vector<Perf> candidates =
      profInit.getProfilePerformanceCandidates(crit[0], cat0, 3);
  float proba = profInit.weightedProbability(candidates[0], crit[0], cat1, cat0,
                                             3, freq, candidates);
  EXPECT_EQ(proba, 6);
}

TEST(TestProfileInitializer, TestInitializeProfilePerformance) {
  Config conf = getTestConf();
  Criteria crit = Criteria(1, "crit");
  crit.generateRandomCriteriaWeights(0);
  std::vector<std::vector<Perf>> perf_vect;
  for (int i = 0; i < 4; i++) {
    std::vector<float> given_perf = {static_cast<float>(0.2 * (i + 1))};
    perf_vect.push_back(
        createVectorPerf("alt" + std::to_string(i), crit, given_perf));
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
  std::vector<Perf> profileCrit =
      profInit.initializeProfilePerformance(crit[0], categories, freq);
  std::ostringstream os;
  os << profileCrit;
  // EXPECT_EQ(os.str(), "[Perf( name : crit0, crit : cat0, value : 0.6
  // ),Perf( name : crit1, crit : cat1, value : 0.8 )]");
  EXPECT_EQ(profileCrit.size(), categories.getNumberCategories() - 1);
}

TEST(TestProfileInitializer, TestInitializeProfiles) {
  int nbAlt = 10;
  Config conf = getTestConf();
  Criteria crit = Criteria(4);
  Categories categories = Categories(3);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", categories[2]}, {"alt1", categories[2]},
          {"alt2", categories[2]}, {"alt3", categories[2]},
          {"alt4", categories[1]}, {"alt5", categories[1]},
          {"alt6", categories[1]}, {"alt7", categories[0]},
          {"alt8", categories[0]}, {"alt9", categories[0]}};

  std::vector<std::vector<Perf>> perf_vect;
  for (int i = 0; i < nbAlt; i++) {
    std::vector<Perf> vp;
    for (int c = 0; c < 4; c++) {
      float given_perf = static_cast<float>(0.2 * (c + i + 1 + 0.06) + 0.1 * i);
      vp.push_back(Perf("alt" + std::to_string(i), "crit" + std::to_string(c),
                        given_perf));
    }
    perf_vect.push_back(vp);
  }
  PerformanceTable perf_tab = PerformanceTable(perf_vect);
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_tab, map);
  ProfileInitializer profInit = ProfileInitializer(conf, alt_perf);
  MRSortModel model = MRSortModel(3, 4);
  profInit.initializeProfiles(model);
  std::cout << model.profiles << std::endl;
  model.profiles.display();
  EXPECT_TRUE(model.profiles.isProfileOrdered());
  // To be uncommented when performance type no longer exist
  model.profiles.changeMode("alt");
  std::cout << model.profiles << std::endl;
  model.profiles.display();
  EXPECT_TRUE(model.profiles.isProfileOrdered());
  model.profiles.changeMode("crit");
  std::cout << model.profiles << std::endl;
  model.profiles.display();
  EXPECT_TRUE(model.profiles.isProfileOrdered());
}
