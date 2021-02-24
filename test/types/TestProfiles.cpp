#include "../../include/types/Criteria.h"
#include "../../include/types/Perf.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/types/Profiles.h"
#include "../../include/utils.h"

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
  Profiles profs = Profiles(12, crit, "b");
  profs.generateRandomPerfValues();
  EXPECT_TRUE(profs.isProfileOrdered());
}

TEST(TestProfiles, TestGetBelowAndAbove) {
  std::vector<Criterion> crit_vect;
  crit_vect.push_back(Criterion("crit0", -1, 0.5));
  crit_vect.push_back(Criterion("crit1", -1, 0.2));
  crit_vect.push_back(Criterion("crit2", -1, 0.3));
  Criteria crit = Criteria(crit_vect);

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> given_perf2 = {0.8, 1, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  std::vector<float> given_perf0 = {0.6, 0, 0.2};
  perf_vect.push_back(createVectorPerf("b0", crit, given_perf0));
  perf_vect.push_back(createVectorPerf("b1", crit, given_perf1));
  perf_vect.push_back(createVectorPerf("b2", crit, given_perf2));

  Profiles profile = Profiles(perf_vect, "alt");

  std::pair<std::vector<Perf>, std::vector<Perf>> profiles1 =
      profile.getBelowAndAboveProfile("b1");
  std::vector<Perf> below1 = profiles1.first;
  std::vector<Perf> above1 = profiles1.second;

  EXPECT_EQ(below1[0].getName(), "b0");
  EXPECT_EQ(above1[0].getName(), "b2");

  std::pair<std::vector<Perf>, std::vector<Perf>> profiles2 =
      profile.getBelowAndAboveProfile("b0");
  std::vector<Perf> below2 = profiles2.first;
  std::vector<Perf> above2 = profiles2.second;

  EXPECT_EQ(below2[0].getName(), "b0");
  EXPECT_EQ(above2[0].getName(), "b1");

  std::pair<std::vector<Perf>, std::vector<Perf>> profiles3 =
      profile.getBelowAndAboveProfile("b2");
  std::vector<Perf> below3 = profiles3.first;
  std::vector<Perf> above3 = profiles3.second;

  EXPECT_EQ(below3[0].getName(), "b1");
  EXPECT_EQ(above3[0].getName(), "b2");
}

TEST(TestProfiles, TestGetBelowAndAboveErrors) {
  std::vector<Criterion> crit_vect;
  crit_vect.push_back(Criterion("crit0", -1, 0.5));
  crit_vect.push_back(Criterion("crit1", -1, 0.2));
  crit_vect.push_back(Criterion("crit2", -1, 0.3));
  Criteria crit = Criteria(crit_vect);

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> given_perf2 = {0.8, 1, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  std::vector<float> given_perf0 = {0.6, 0, 0.2};
  perf_vect.push_back(createVectorPerf("b0", crit, given_perf0));
  perf_vect.push_back(createVectorPerf("b1", crit, given_perf1));
  perf_vect.push_back(createVectorPerf("b2", crit, given_perf2));
  Profiles profile = Profiles(perf_vect, "alt");

  // TEST INVALID ARGUMENT
  try {
    std::pair<std::vector<Perf>, std::vector<Perf>> profiles1 =
        profile.getBelowAndAboveProfile("b3");
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Profile not found."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }

  // TEST PROFILE MODE IS NOT ALT
  profile.changeMode("crit");
  try {
    std::pair<std::vector<Perf>, std::vector<Perf>> profiles1 =
        profile.getBelowAndAboveProfile("b1");
    FAIL() << "should have thrown domain error.";
  } catch (std::domain_error const &err) {
    EXPECT_EQ(err.what(), std::string("Profiles perftable mode corrupted."));
  } catch (...) {
    FAIL() << "should have thrown domain error.";
  }
}

TEST(TestProfiles, TestSetPerfAndErrors) {
  std::vector<Criterion> crit_vect;
  crit_vect.push_back(Criterion("crit0", -1, 0.5));
  crit_vect.push_back(Criterion("crit1", -1, 0.2));
  crit_vect.push_back(Criterion("crit2", -1, 0.3));
  Criteria crit = Criteria(crit_vect);

  std::vector<std::vector<Perf>> perf_vect;
  std::vector<float> given_perf2 = {0.8, 1, 0.4};
  std::vector<float> given_perf1 = {0.8, 0.1, 0.3};
  std::vector<float> given_perf0 = {0.6, 0, 0.2};
  perf_vect.push_back(createVectorPerf("b0", crit, given_perf0));
  perf_vect.push_back(createVectorPerf("b1", crit, given_perf1));
  perf_vect.push_back(createVectorPerf("b2", crit, given_perf2));
  Profiles profile = Profiles(perf_vect, "alt");

  profile.setPerf("b0", "crit1", 0.98);

  EXPECT_FLOAT_EQ(profile.getPerf("b0", "crit1").getValue(), 0.98);

  // TEST ERRORS
  try {
    profile.setPerf("b22", "crit1", 0.98);
    FAIL() << "should have thrown domain error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Name not found in performance table"));
  } catch (...) {
    FAIL() << "should have thrown domain error.";
  }

  try {
    profile.setPerf("b0", "crit31", 0.98);
    FAIL() << "should have thrown domain error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Criterion not found in performance table"));
  } catch (...) {
    FAIL() << "should have thrown domain error.";
  }
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
