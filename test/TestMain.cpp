#include "learning/TestInitializeProfile.cpp"
// #include "TestUtils.cpp"
// #include "types/TestAlternativesPerformance.cpp"
// #include "types/TestCategories.cpp"
// #include "types/TestCategory.cpp"
// #include "types/TestCriteria.cpp"
// #include "types/TestCriterion.cpp"
// #include "types/TestDataGenerator.cpp"
// #include "types/TestMRSortModel.cpp"
// #include "types/TestPerf.cpp"
// #include "types/TestPerformance.cpp"
// #include "types/TestPerformanceTable.cpp"
#include "types/TestProfiles.cpp"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

int main(int argc, char *argv[]) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}