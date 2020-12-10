#include "TestCategories.cpp"
#include "TestCategory.cpp"
#include "TestCriteria.cpp"
#include "TestCriterion.cpp"
#include "TestPerformance.cpp"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

int AtomicMCDAObject::nb_instances_{0};

int main(int argc, char *argv[]) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}