#include "../../include/types/Criteria.h"
#include "../../include/types/Perf.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/types/Profiles.h"

#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestProfiles, TestConstructorWithPerfVectError) {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(2, "a");
  std::vector<float> given_perf0 = {0.6, 1};
  std::vector<float> given_perf1 = {0.8, 0.1};
  perf_vect.push_back(createVectorPerf("test0", crit, given_perf0));
  perf_vect.push_back(createVectorPerf("test1", crit, given_perf1));
  try {
    Profiles profs = Profiles(perf_vect, "alt");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Profile in its given profile is not ordered"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestProfiles, TestConstructorWithoutPerfVect) {
  Criteria crit = Criteria(4, "crit");
  Profiles profs = Profiles(12, crit, "alt", "b");
  EXPECT_TRUE(profs.isProfileOrdered());

  Profiles profs2 = Profiles(12, crit, "crit", "criterion");
  EXPECT_TRUE(profs2.isProfileOrdered());
}

TEST(TestProfiles, TestIsOrdered) {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(3, "crit");
  std::vector<float> given_perf0 = {0.1, 0.2, 0.3};
  std::vector<float> given_perf1 = {0.3, 0.5, 0.6};
  perf_vect.push_back(createVectorPerf("test0", crit, given_perf0));
  perf_vect.push_back(createVectorPerf("test1", crit, given_perf1));
  Profiles profile = Profiles(perf_vect, "alt");
  EXPECT_TRUE(profile.isProfileOrdered());

  std::vector<float> given_perf2 = {0.6, 0, 0.5};
  perf_vect.push_back(createVectorPerf("test2", crit, given_perf2));
  try {
    Profiles profile2 = Profiles(perf_vect, "alt");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Profile in its given profile is not ordered"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestProfiles, TestGenerateRandomOrderedPerfValues) {
  Criteria crit = Criteria(4, "crit");
  Profiles profs = Profiles(12, crit, "alt", "b");
  profs.generateRandomPerfValues();
  EXPECT_TRUE(profs.isProfileOrdered());

  Profiles profs2 = Profiles(12, crit, "crit", "criterion");
  profs2.generateRandomPerfValues();
  EXPECT_TRUE(profs2.isProfileOrdered());
}

TEST(TestProfiles, TestChangeMode) {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(3, "crit");
  std::vector<float> given_perf0 = {1.1, 0.8, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.3, 0.1};
  perf_vect.push_back(createVectorPerf("alt0", crit, given_perf1));
  perf_vect.push_back(createVectorPerf("alt1", crit, given_perf0));
  Profiles profile = Profiles(perf_vect, "alt");
  profile.changeMode("crit");
  EXPECT_TRUE(profile.isProfileOrdered());
}
