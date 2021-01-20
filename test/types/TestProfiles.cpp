#include "../../include/types/Criteria.h"
#include "../../include/types/Performance.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/types/Profiles.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestProfiles, TestConstructorWithPerfVectError) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  std::vector<float> given_perf0 = {0.6, 1};
  std::vector<float> given_perf1 = {0.8, 0.1};
  perf_vect.push_back(Performance(crit, given_perf0, "test0"));
  perf_vect.push_back(Performance(crit, given_perf1, "test1"));
  try {
    Profiles profs = Profiles(perf_vect, "alt");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(
        err.what(),
        std::string(
            "The given performance vector cannot be used as a profiles "
            "performance table. Each row must be ranked such that for each row "
            "i we have on each criterion j : v_i - 1_j > v_i_j > v_i + 1_j"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestProfiles, TestIsOrdered) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(3, "crit");
  std::vector<float> given_perf0 = {0.8, 0.9, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  perf_vect.push_back(Performance(crit, given_perf0, "alt0"));
  perf_vect.push_back(Performance(crit, given_perf1, "alt1"));
  Profiles profile = Profiles(perf_vect, "alt");
  EXPECT_TRUE(profile.isProfileOrdered());

  std::vector<float> given_perf2 = {0.6, 0, 0.5};
  perf_vect.push_back(Performance(crit, given_perf2, "test2"));
  try {
    Profiles profile2 = Profiles(perf_vect, "alt");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(
        err.what(),
        std::string(
            "The given performance vector cannot be used as a profiles "
            "performance table. Each row must be ranked such that for each row "
            "i we have on each criterion j : v_i - 1_j > v_i_j > v_i + 1_j"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestProfiles, TestGenerateRandomOrderedPerfValues) {
  Criteria crit = Criteria(4, "crit");
  Profiles profs = Profiles(12, crit, "alt");
  profs.generateRandomPerfValues();
  EXPECT_TRUE(profs.isProfileOrdered());
}

TEST(TestProfiles, TestChangeMode) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(3, "crit");
  std::vector<float> given_perf0 = {1.1, 0.8, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.3, 0.1};
  perf_vect.push_back(Performance(crit, given_perf0, "alt0"));
  perf_vect.push_back(Performance(crit, given_perf1, "alt1"));
  Profiles profile = Profiles(perf_vect, "alt");
  Profiles new_p = profile.changeMode();
  EXPECT_TRUE(new_p.isProfileOrdered());
}
