#include "../src/DataGenerator.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

// TEST(TestDataGenerator, TestDatasetGenerator) {
//   DataGenerator data = DataGenerator();
//   data.DatasetGenerator(3, 20, 4, "test.xml");
// }

// TEST(TestDataGenerator, TestModelGenerator) {
//   DataGenerator data = DataGenerator();
//   data.modelGenerator(2, 3, "test_model.xml");
// }

TEST(TestDataGenerator, TestNumberOfCriteriaForModels) {
  DataGenerator data = DataGenerator();
  int crit = data.getNumberOfCriteria("test_model.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "2");
}

TEST(TestDataGenerator, TestNumberOfCriteriaForData) {
  DataGenerator data = DataGenerator();
  int crit = data.getNumberOfCriteria("test.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "3");
}

TEST(TestDataGenerator, TestNumberOfCategoriesForModels) {
  DataGenerator data = DataGenerator();
  int crit = data.getNumberOfCategories("test_model.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "3");
}

TEST(TestDataGenerator, TestNumberOfCategoriesForData) {
  DataGenerator data = DataGenerator();
  int crit = data.getNumberOfCategories("test.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "4");
}
