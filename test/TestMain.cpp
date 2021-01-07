#include "types/TestCategories.cpp"
#include "types/TestCategory.cpp"
#include "types/TestCriteria.cpp"
#include "types/TestCriterion.cpp"
#include "types/TestPerf.cpp"
#include "types/TestPerformance.cpp"
#include "types/TestPerformanceTable.cpp"
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <utility>

int AtomicMCDAObject::nb_instances_{0};

int main(int argc, char *argv[]) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(TestDataGenerator, TestDummy) {
  std::ofstream file;
  file.open("../data/test.txt");
  std::cout << file.is_open() << std::endl;
  file
      << "Please writr this text to a file.\n this text is written using C++\n";
  file.close();
  EXPECT_EQ("0", "1");
}