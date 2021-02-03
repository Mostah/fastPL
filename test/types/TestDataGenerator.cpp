#include "../../include/app.h"
#include "../../include/types/DataGenerator.h"
#include "../../include/utils.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>

// Config getTestConf() {
//   Config conf;
//   conf.data_dir = "../data/tests/";
//   return conf;
// }

TEST(TestDataGenerator, TestDatasetGenerator) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  data.datasetGenerator(3, 5, 4, "test_dataset.xml", 1, 0);
  bool u = fileExists(conf.data_dir + "test_dataset.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

TEST(TestDataGenerator, TestDatasetGeneratorNotOverwrite) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    data.datasetGenerator(3, 5, 4, "test_dataset.xml", 0, 0);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  data.datasetGenerator(3, 5, 4, "test_dataset.xml", 1, 0);
  bool u = fileExists(conf.data_dir + "test_dataset.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

/**
TEST(TestDataGenerator, TestModelGenerator) {
    Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  data.modelGenerator(2, 3, "test_model.xml", 1, 0);
  bool u = fileExists(conf.data_dir+"test_model.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}
*/

TEST(TestDataGenerator, TestModelGeneratorNotOverwrite) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::string type = data.getXmlFileType("test_model.xml");
  std::ostringstream os2;
  os2 << type;
  EXPECT_EQ(os2.str(), "model");
}

TEST(TestDataGenerator, TestXmlFileType2) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::string type = data.getXmlFileType("test_dataset.xml");
  std::ostringstream os2;
  os2 << type;
  EXPECT_EQ(os2.str(), "dataset");
}

TEST(TestDataGenerator, TestXmlFileType3) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    std::string type = data.getXmlFileType("test_wrong_structure.xml");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Cannot open xml file, please check path"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestLoadModel) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
            "PerformanceTable[ Performance: Perf( name : prof0, crit : crit0, "
            "value : 0.93 ) Perf( name : prof0, crit : crit1, value : 0.93 ) | "
            "Performance: Perf( name : prof1, crit : crit0, value : 0.93 ) "
            "Perf( name : prof1, crit : crit1, value : 0.93 ) | Performance: "
            "Perf( name : prof2, crit : crit0, value : 0.93 ) Perf( name : "
            "prof2, crit : crit1, value : 0.93 ) | ]");
}

TEST(TestDataGenerator, TestSaveModelWrongNumberCriteria) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  Criteria crit = Criteria(2, "a");
  crit.generateRandomCriteriaWeights(0);
  float lambda = 0.1;
  PerformanceTable perf_table = PerformanceTable(2, crit, "test");
  data.saveModel("test_save_model.xml", lambda, crit, perf_table, 1);
  bool u = fileExists(conf.data_dir + "test_save_model.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

TEST(TestDataGenerator, TestSaveModelCanttOverwrite) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  std::string path_to_remove = conf.data_dir + "test_save_model.xml";
  std::remove(path_to_remove.c_str());
}

TEST(TestDataGenerator, TestSaveDataset) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  Criteria crit = Criteria(3, "crit");
  crit.generateRandomCriteriaWeights(0);
  PerformanceTable perf_table = PerformanceTable(4, crit, "alt");
  perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0}, {"alt1", cat1}, {"alt2", cat0}, {"alt3", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  data.saveDataset("test_save_dataset.xml", alt_perf, 2, 0);
  bool u = fileExists(conf.data_dir + "dataset_alt4_crit3_cat2.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

TEST(TestDataGenerator, TestSaveDatasetCanOverwrite) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  Criteria crit = Criteria(3, "crit");
  crit.generateRandomCriteriaWeights(0);
  PerformanceTable perf_table = PerformanceTable(4, crit, "alt");
  perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0}, {"alt1", cat1}, {"alt2", cat0}, {"alt3", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);

  data.saveDataset("test_save_dataset.xml", alt_perf, 2, 1);
  bool u = fileExists(conf.data_dir + "dataset_alt4_crit3_cat2.xml");
  std::ostringstream os2;
  os2 << u;
  EXPECT_EQ(os2.str(), "1");
}

TEST(TestDataGenerator, TestSaveDatasetCantOverwrite) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  Criteria crit = Criteria(3, "crit");
  crit.generateRandomCriteriaWeights(0);
  PerformanceTable perf_table = PerformanceTable(4, crit, "alt");
  perf_table.generateRandomPerfValues();
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{
          {"alt0", cat0}, {"alt1", cat1}, {"alt2", cat0}, {"alt3", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
  try {
    data.saveDataset("test_save_dataset.xml", alt_perf, 2, 0);
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Such a default xml generate (or not) filename "
                          "already exists and you chose not to overwrite it"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
  std::string path_to_remove = conf.data_dir + "dataset_alt4_crit3_cat2.xml";
  std::remove(path_to_remove.c_str());
}

/*
TEST(TestDataGenerator, TestLoadDataset) {
    Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::tuple<PerformanceTable, std::unordered_map<std::string, Category>> t =
      data.loadDataset(conf.data_dir+"test.xml");
  PerformanceTable pt = std::get<0>(t);
  std::unordered_map<std::string, Category> altAssignments = std::get<1>(t);
  AlternativesPerformance alt_perf =
      AlternativesPerformance(pt, altAssignments);
  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(
      os.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "alt0, crit : crit0, value : 0 ) Perf( name : alt0, crit : crit1, value "
      ": 0 ) Perf( name : alt0, crit : crit2, value : 0 ) | Performance: Perf( "
      "name : alt1, crit : crit0, value : 0 ) Perf( name : alt1, crit : crit1, "
      "value : 0 ) Perf( name : alt1, crit : crit2, value : 0 ) | Performance: "
      "Perf( name : alt2, crit : crit0, value : 0 ) Perf( name : alt2, crit : "
      "crit1, value : 0 ) Perf( name : alt2, crit : crit2, value : 0 ) | "
      "Performance: Perf( name : alt3, crit : crit0, value : 0 ) Perf( name : "
      "alt3, crit : crit1, value : 0 ) Perf( name : alt3, crit : crit2, value "
      ": 0 ) | Performance: Perf( name : alt4, crit : crit0, value : 0 ) Perf( "
      "name : alt4, crit : crit1, value : 0 ) Perf( name : alt4, crit : crit2, "
      "value : 0 ) | ], AlternativesAssignment{ alt4->Category(id : , rank : "
      "-1) alt3->Category(id : , rank : -1) alt2->Category(id : , rank : -1) "
      "alt0->Category(id : , rank : -1) alt1->Category(id : , rank : -1) }");
}
**/

TEST(TestDataGenerator, TestNumberOfCriteriaForModels) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  int crit = data.getNumberOfCriteria("test_model.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "2");
}

TEST(TestDataGenerator, TestNumberOfCriteriaForData) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  int crit = data.getNumberOfCriteria("test_dataset.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "3");
}

TEST(TestDataGenerator, TestNumberOfCategoriesForModels) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  int crit = data.getNumberOfCategories("test_model.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "3");
}

TEST(TestDataGenerator, TestNumberOfCategoriesForData) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  int crit = data.getNumberOfCategories("test_dataset.xml");
  std::ostringstream os2;
  os2 << crit;
  EXPECT_EQ(os2.str(), "4");
}

TEST(TestDataGenerator, TestGetLambdaForData) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    int crit = data.getThresholdValue("test_dataset.xml");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find any threshold in xml file, "
                                      "most likely have a xml dataset file"));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetLambdaForModel) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  float lambda = data.getThresholdValue("test_model.xml");
  std::ostringstream os2;
  os2 << lambda;
  EXPECT_EQ(os2.str(), "0.9035");
}

TEST(TestDataGenerator, TestGetNumerOfAlternativesForDataset) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  int nb_alternatives = data.getNumberOfAlternatives("test_dataset.xml");
  std::ostringstream os2;
  os2 << nb_alternatives;
  EXPECT_EQ(os2.str(), "5");
}

TEST(TestDataGenerator, TestGetNumerOfAlternativesForModel) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  Performance p = data.getAlternativePerformance("test_dataset.xml", "alt0");
  std::ostringstream os2;
  os2 << p;
  EXPECT_EQ(os2.str(), "Performance(Perf( name : alt0, crit : crit0, value : 0 "
                       "), Perf( name : alt0, crit : crit1, value : 0 ), Perf( "
                       "name : alt0, crit : crit2, value : 0 ), )");
}

TEST(TestDataGenerator, TestGetAlternativePerformanceForModel) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    Performance p = data.getAlternativePerformance("test_dataset.xml", "hello");
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::vector<std::string> v = data.getAlternativeIds("test_dataset.xml");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[alt0,alt1,alt2,alt3,alt4]");
}

TEST(TestDataGenerator, TestGetAlternativeIdsForModel) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::vector<std::string> v = data.getCriteriaIds("test_dataset.xml");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[crit0,crit1,crit2]");
}

TEST(TestDataGenerator, TestGetCriteriaIdsForModel) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::vector<std::string> v = data.getCriteriaIds("test_model.xml");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[crit0,crit1]");
}

TEST(TestDataGenerator, TestGetCriterionForDataset) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    Criterion v = data.getCriterion("test_dataset.xml", "crit1");
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  Criterion crit1 = data.getCriterion("test_model.xml", "crit1");
  std::ostringstream os2;
  os2 << crit1;
  EXPECT_EQ(os2.str(), "Criterion(id : crit1, direction : +, weight : 0.5)");
}

TEST(TestDataGenerator, TestGetCriterionForModelFakeCritId) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  int v = data.getAlternativeAssignment("test_dataset.xml", "alt0");
  std::ostringstream os2;
  os2 << v;
  // need to modify that when new type is in
  EXPECT_EQ(os2.str(), "0");
}

TEST(TestDataGenerator, TestGetAlternativeAssignmentDatasetFakeAltId) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    int v = data.getAlternativeAssignment("test_dataset.xml", "hello");
    FAIL() << "should have throw invalid_argument error.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Cannot find category assignment "
                                      "associated to alt_id in xml file."));
  } catch (...) {
    FAIL() << "should have throw invalid_argument error.";
  }
}

TEST(TestDataGenerator, TestGetCriterionCategoryLimitsDataset) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  try {
    std::vector<float> v =
        data.getCriterionCategoryLimits("test_dataset.xml", "crit1");
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
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
  std::vector<float> v =
      data.getCriterionCategoryLimits("test_model.xml", "crit0");
  std::ostringstream os2;
  os2 << v;
  EXPECT_EQ(os2.str(), "[0.93,0.93,0.93]");
}

TEST(TestDataGenerator, TestGetCriterionCategoryLimitsModelFakeCritId) {
  Config conf = getTestConf();
  DataGenerator data = DataGenerator(conf);
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
