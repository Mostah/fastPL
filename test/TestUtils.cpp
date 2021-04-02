#include "../include/app.h"
#include "../include/learning/ProfileInitializer.h"
#include "../include/types/DataGenerator.h"
#include "../include/types/MRSortModel.h"
#include "../include/utils.h"
#include "gtest/gtest.h"
#include <iostream>
#include <ostream>
#include <utility>

TEST(TestUtils, TestRandomIntGenerator) {
  int random1 = getRandomUniformInt(0, 0, 20);
  int random2 = getRandomUniformInt(0, 0, 20);
  int random3 = getRandomUniformInt(40, 0, 20);

  int random4 = getRandomUniformInt();

  EXPECT_EQ(random1, random2);
  EXPECT_NE(random1, random3);

  EXPECT_TRUE(random4 >= 0 && random4 <= 100);
}

TEST(TestUtils, TestRandomFloatGenerator) {
  float random1 = getRandomUniformFloat(0, 1, 4);
  float random2 = getRandomUniformFloat(0, 1, 4);
  float random3 = getRandomUniformFloat(40, 1, 4);

  float random4 = getRandomUniformFloat();

  EXPECT_EQ(random1, random2);
  EXPECT_NE(random1, random3);
  EXPECT_TRUE(random4 >= 0 && random4 <= 1);
}

TEST(TestUtils, TestUtilsGetPerfOfCrit) {
  std::string id = "alt1";
  Criteria crit = Criteria(3);
  std::vector<float> givenPerf = {1, 3, 4};
  std::vector<Perf> vecp = createVectorPerf(id, crit, givenPerf);
  Perf p = getPerfOfCrit(vecp, "crit1");
  EXPECT_EQ(p.value_, 3);
  try {
    Perf p = getPerfOfCrit(vecp, "crit6");
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("No performance for given criterion found"));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}

TEST(TestUtils, TestUtilsVectorPerf) {
  std::string id = "alt1";
  Criteria crit = Criteria(3);
  std::vector<float> givenPerf = {1, 3, 4};
  std::vector<Perf> vecp = createVectorPerf(id, crit, givenPerf);
  std::ostringstream os;
  os << vecp;
  EXPECT_EQ(
      os.str(),
      "[Perf( name : alt1, crit : crit0, value : 1 ),Perf( name : alt1, crit : "
      "crit1, value : 3 ),Perf( name : alt1, crit : crit2, value : 4 )]");
}

// TEST(TestUtils, TestUtilsPlotData) {
//   Config conf = getTestConf();
//   DataGenerator data = DataGenerator(conf);
//   std::string filename = "in7dataset.xml";
//   AlternativesPerformance ap = data.loadDataset(filename);
//   int nbCat = data.getNumberOfCategories(filename);
//   int nbCrit = data.getNumberOfCriteria(filename);
//   MRSortModel mr = MRSortModel(nbCat, nbCrit);
//   plotGlobalData(ap);
//   ProfileInitializer profInit = ProfileInitializer(conf, ap);
//   profInit.initializeProfiles(mr);
// works if we take out profile mode condition in function or
//     fix change mode bug
// mr.profiles.changeMode("alt");
// mr.profiles.changeMode("crit");
// plotProfile(mr.profiles);
//}
