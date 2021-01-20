#include "learning/TestInitializeProfile.cpp"
// #include "types/TestAlternativesPerformance.cpp"
// #include "types/TestCategories.cpp"
// #include "types/TestCategory.cpp"
// #include "types/TestCriteria.cpp"
// #include "types/TestCriterion.cpp"
// #include "types/TestDataGenerator.cpp"
// #include "types/TestPerf.cpp"
// #include "types/TestPerformance.cpp"
// #include "types/TestPerformanceTable.cpp"
#include "TestUtils.cpp"
#include "types/TestAlternativesPerformance.cpp"
#include "types/TestCategories.cpp"
#include "types/TestCategory.cpp"
#include "types/TestCriteria.cpp"
#include "types/TestCriterion.cpp"
#include "types/TestDataGenerator.cpp"
#include "types/TestPerf.cpp"
#include "types/TestPerformance.cpp"
#include "types/TestPerformanceTable.cpp"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

int AtomicMCDAObject::nb_instances_{0};

int main(int argc, char *argv[]) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}