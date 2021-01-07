#include "../src/DataGenerator.h"
#include "../src/utils.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

TEST(TestDataGenerator, TestDummy) {
  std::ofstream file;
  file.open("../data/test.txt");
  file
      << "Please writr this text to a file.\n this text is written using C++\n";
  file.close();
  bool u = fileExists("../data/test.txt");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

/**
TEST(TestDataGenerator, TestDatasetGenerator) {
  DataGenerator data = DataGenerator();
  data.datasetGenerator(3, 20, 4, "test.xml", 1, 0);
  bool u = fileExists("../data/test.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}
*/

TEST(TestDataGenerator, TestDatasetGeneratorNotOverwrite) {
  DataGenerator data = DataGenerator();
  try {
    data.datasetGenerator(3, 20, 4, "test.xml", 0, 0);
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Such a default xml generate (or not) filename "
                          "already exists and you chose "
                          "not to overwrite it"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestDatasetGeneratorOverwrite) {
  DataGenerator data = DataGenerator();
  data.datasetGenerator(3, 20, 4, "test.xml", 1, 0);
  bool u = fileExists("../data/test.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

/**
TEST(TestDataGenerator, TestModelGenerator) {
  DataGenerator data = DataGenerator();
  data.modelGenerator(2, 3, "test_model.xml", 1, 0);
  bool u = fileExists("../data/test_model.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}
*/

TEST(TestDataGenerator, TestModelGeneratorNotOverwrite) {
  DataGenerator data = DataGenerator();
  try {
    data.modelGenerator(2, 3, "test_model.xml", 0, 0);
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Such a default xml generate (or not) filename "
                          "already exists and you chose "
                          "not to overwrite it"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestXmlFileType) {
  DataGenerator data = DataGenerator();
  std::string type = data.getXmlFileType("test_model.xml");
  std::ostringstream os2;
  os2 << type;
  EXPECT_EQ(os2.str(), "model");
}

TEST(TestDataGenerator, TestXmlFileType2) {
  DataGenerator data = DataGenerator();
  std::string type = data.getXmlFileType("test.xml");
  std::ostringstream os2;
  os2 << type;
  EXPECT_EQ(os2.str(), "dataset");
}

TEST(TestDataGenerator, TestXmlFileType3) {
  DataGenerator data = DataGenerator();
  try {
    std::string type = data.getXmlFileType("wrong_structure.xml");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot open xml file, please check path"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestLoadModel) {
  DataGenerator data = DataGenerator();
  std::tuple<float, Criteria, PerformanceTable> t =
      data.loadModel("test_model.xml");
  float lambda = std::get<0>(t);
  Criteria crit = std::get<1>(t);
  PerformanceTable pt = std::get<2>(t);
  std::ostringstream os;
  std::ostringstream os2;
  std::ostringstream os3;
  os << lambda;
  os2 << crit;
  os3 << pt;
  EXPECT_EQ(os.str(), "0.9035");
  EXPECT_EQ(os2.str(),
            "Criteria(Criterion(id : crit0, direction : +, weight : 0.5), "
            "Criterion(id : crit1, direction : +, weight : 0.5), )");
  EXPECT_EQ(os3.str(),
            "PerformanceTable(Performance(Perf( name : prof0, crit : crit0, "
            "value : 0.93 ), Perf( name : prof0, crit : crit1, value : 0.93 ), "
            "), Performance(Perf( name : prof1, crit : crit0, value : 0.93 ), "
            "Perf( name : prof1, crit : crit1, value : 0.93 ), ), "
            "Performance(Perf( name : prof2, crit : crit0, value : 0.93 ), "
            "Perf( name : prof2, crit : crit1, value : 0.93 ), ), )");
}

TEST(TestDataGenerator, TestSaveModelWrongNumberCriteria) {
  DataGenerator data = DataGenerator();
  Criteria crit = Criteria(2, "a");
  float lambda = 0.1;
  PerformanceTable perf_table = PerformanceTable(3, crit, "test");
  try {
    data.saveModel("test_save_model.xml", lambda, crit, perf_table, 1);
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string(" Number of criteria and the number and the length "
                          "of the performance of the fictive alternative (ie "
                          "profile performance) does not match"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestSaveModel) {
  DataGenerator data = DataGenerator();
  Criteria crit = Criteria(2, "a");
  crit.generateRandomCriteriaWeights(0);
  float lambda = 0.1;
  PerformanceTable perf_table = PerformanceTable(2, crit, "test");
  perf_table.generateRandomPerfValues(1);
  data.saveModel("test_save_model.xml", lambda, crit, perf_table, 1);
}

TEST(TestDataGenerator, TestSaveModelCanttOverwrite) {
  DataGenerator data = DataGenerator();
  Criteria crit = Criteria(2, "a");
  float lambda = 0.1;
  PerformanceTable perf_table = PerformanceTable(2, crit, "test");
  try {
    data.saveModel("test_save_model.xml", lambda, crit, perf_table, 0);
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Such a default xml generate (or not) filename "
                          "already exists and you chose not to overwrite it"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

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

TEST(TestDataGenerator, TestGetLambdaForData) {
  DataGenerator data = DataGenerator();
  try {
    int crit = data.getThresholdValue("test.xml");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find any threshold in xml file, "
                                      "most likely have a xml dataset file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetLambdaForModel) {
  DataGenerator data = DataGenerator();
  float lambda = data.getThresholdValue("test_model.xml");
  std::ostringstream os2;
  os2 << lambda;
  EXPECT_EQ(os2.str(), "0.9035");
}

TEST(TestDataGenerator, TestGetNumerOfAlternativesForDataset) {
  DataGenerator data = DataGenerator();
  int nb_alternatives = data.getNumberOfAlternatives("test.xml");
  std::ostringstream os2;
  os2 << nb_alternatives;
  EXPECT_EQ(os2.str(), "20");
}

TEST(TestDataGenerator, TestGetNumerOfAlternativesForModel) {
  DataGenerator data = DataGenerator();
  try {
    int nb_alt = data.getNumberOfAlternatives("test_model.xml");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot find the number of alternatives in xml file, "
                          "most likely have a xml model file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetAlternativePerformanceForDataset) {
  DataGenerator data = DataGenerator();
  Performance p = data.getAlternativePerformance("test.xml", "alt0");
  std::ostringstream os2;
  os2 << p;
  EXPECT_EQ(os2.str(), "Performance(Perf( name : alt0, crit : crit0, value : 0 "
                       "), Perf( name : alt0, crit : crit1, value : 0 ), Perf( "
                       "name : alt0, crit : crit2, value : 0 ), )");
}

TEST(TestDataGenerator, TestGetAlternativePerformanceForModel) {
  DataGenerator data = DataGenerator();
  try {
    Performance p = data.getAlternativePerformance("test_model.xml", "alt0");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot find any alternatives in xml file, "
                          "most likely have a xml model file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetAlternativePerformanceForModelFakeAltId) {
  DataGenerator data = DataGenerator();
  try {
    Performance p = data.getAlternativePerformance("test.xml", "hello");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot find performance associated to the "
                          "alternative identified by alt_id in xml file."));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetAlternativeIdsForDataset) {
  DataGenerator data = DataGenerator();
  std::vector<std::string> v = data.getAlternativeIds("test.xml");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(),
            "[alt0,alt1,alt2,alt3,alt4,alt5,alt6,alt7,alt8,alt9,alt10,"
            "alt11,alt12,alt13,alt14,alt15,alt16,alt17,alt18,alt19]");
}

TEST(TestDataGenerator, TestGetAlternativeIdsForModel) {
  DataGenerator data = DataGenerator();
  try {
    std::vector<std::string> v = data.getAlternativeIds("test_model.xml");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot find any alternatives in xml file, "
                          "most likely have a xml model file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetCriteriaIdsForDataset) {
  DataGenerator data = DataGenerator();
  std::vector<std::string> v = data.getCriteriaIds("test.xml");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[crit0,crit1,crit2]");
}

TEST(TestDataGenerator, TestGetCriteriaIdsForModel) {
  DataGenerator data = DataGenerator();
  std::vector<std::string> v = data.getCriteriaIds("test_model.xml");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[crit0,crit1]");
}

TEST(TestDataGenerator, TestGetCriterionForDataset) {
  DataGenerator data = DataGenerator();
  try {
    Criterion v = data.getCriterion("test.xml", "crit1");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find the criterion associated "
                                      "to its weights in xml file, "
                                      "most likely have a xml model file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetCriterionForModel) {
  DataGenerator data = DataGenerator();
  Criterion crit1 = data.getCriterion("test_model.xml", "crit1");
  std::ostringstream os2;
  os2 << crit1;
  EXPECT_EQ(os2.str(), "Criterion(id : crit1, direction : +, weight : 0.5)");
}

TEST(TestDataGenerator, TestGetCriterionForModelFakeCritId) {
  DataGenerator data = DataGenerator();
  try {
    Criterion crit1 = data.getCriterion("test_model.xml", "testing");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string(
                  "Cannot find Criterion associated to crit_id in xml file."));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetAlternativeAssignmentModel) {
  DataGenerator data = DataGenerator();
  try {
    int v = data.getAlternativeAssignment("test_model.xml", "alt0");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find category assignment "
                                      "associated to alternative in xml file, "
                                      "most likely have a xml model file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetAlternativeAssignmentDataset) {
  DataGenerator data = DataGenerator();
  int v = data.getAlternativeAssignment("test.xml", "alt0");
  std::ostringstream os2;
  os2 << v;
  // need to modify that when new type is in
  EXPECT_EQ(os2.str(), "0");
}

TEST(TestDataGenerator, TestGetAlternativeAssignmentDatasetFakeAltId) {
  DataGenerator data = DataGenerator();
  try {
    int v = data.getAlternativeAssignment("test.xml", "hello");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find category assignment "
                                      "associated to alt_id in xml file."));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetCriterionCategoryLimitsDataset) {
  DataGenerator data = DataGenerator();
  try {
    std::vector<float> v = data.getCriterionCategoryLimits("test.xml", "crit1");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot find category limits associated to "
                          "criterion in xml file, "
                          "most likely have a xml dataset file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetCriterionCategoryLimitsModel) {
  DataGenerator data = DataGenerator();
  std::vector<float> v =
      data.getCriterionCategoryLimits("test_model.xml", "crit0");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[0.93,0.93,0.93]");
}

TEST(TestDataGenerator, TestGetCriterionCategoryLimitsModelFakeCritId) {
  DataGenerator data = DataGenerator();
  try {
    std::vector<float> v =
        data.getCriterionCategoryLimits("test_model.xml", "hello");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find criterion category limit "
                                      "associated to crit_id in xml file."));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}
