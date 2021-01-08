#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

// will be using pugiXML for parsing and generating
// git clone https://github.com/zeux/pugixml.git
// forked repo and uncomment in pugixml/src/pugiconfig.hpp
// line 48 #define PUGIXML_HEADER_ONLY

#include "../../extsrc/pugixml/src/pugixml.hpp"
#include "AtomicMCDAObject.h"
#include "Criterion.h"
#include "Performance.h"
#include "PerformanceTable.h"
#include <iostream>
#include <tuple>

class DataGenerator : public AtomicMCDAObject {
public:
  DataGenerator(){};

  /**
   * Generate a random dataset and put it in a xml file
   *
   * @param nb_criteria number of criteria
   * @param nb_alternative number of alternatives
   * @param nb_categories number of categories
   * @param datasetName name of that dataset
   * @param overwrite bool that refers to if we want to overwrite file
   * @param seed seed to control random generation
   *
   * @return a saved file of the name datasetName.xml in data/ or default
   * generated name
   */
  void datasetGenerator(int nb_criteria, int nb_alternative, int nb_categories,
                        std::string datasetName = "", bool overwrite = 1,
                        bool changeSeed = 0);

  /**
   * Generate a random model and puts it in a xml file
   *
   * @param nb_criteria number of criteria
   * @param nb_categories number of categories
   * @param modelName name of that model
   * @param overwrite bool that refers to if we want to overwrite file
   * @param seed seed to control random generation
   *
   * @return a saved file of the name nametName.xml in data/ or default
   * generated name
   */
  void modelGenerator(int nb_criteria, int nb_categories, std::string modelName,
                      bool overwrite = 1, bool changeSeed = 0);

  /**
   * Get data from xml file
   *
   * @param fileName filename
   *
   * @return a type for model usage
   *
   */
  // void loadDataset(std::string fileName);

  // void saveDataset(new type, std::string datasetName);

  /**
   * Get model data from xml file
   *
   * @param fileName filename
   *
   * @return object to use algorithms
   *
   */
  std::tuple<float, Criteria, PerformanceTable> loadModel(std::string fileName);

  /**
   * Save model data in xml file name filename
   *
   * @param fileName filename
   *
   * @return save file in xml format
   *
   */
  void saveModel(std::string fileName, float lambda, Criteria criteria,
                 PerformanceTable pt, bool overwrite = 1,
                 std::string modelName = "");

  /**
   * Get an xml_document type for preocessing the xml files
   *
   * @param fileName filename
   *
   * @return pugi::xml_document object that hold info on the xml filename
   * file
   */
  pugi::xml_document openXmlFile(std::string filename);

  /**
   * Gets the xml file type
   *
   * @param fileName filename
   *
   * @return "model" for model xml file types or "dataset" for data xml
   */
  std::string getXmlFileType(std::string fileName);

  /**
   * Get number of criterias found in xml file
   *
   * @param fileName filename
   *
   */
  int getNumberOfCriteria(std::string fileName);

  /**
   * Get number of categories found in xml file
   *
   * @param fileName filename
   *
   */
  int getNumberOfCategories(std::string fileName);

  /**
   * Get lambda threshold value found in xml file if it exists
   *
   * @param fileName filename
   *
   */
  float getThresholdValue(std::string fileName);

  /**
   * Get number of alternatives found in dataset xml file if it exists
   *
   * @param fileName filename
   *
   */
  int getNumberOfAlternatives(std::string fileName);

  /**
   * Get performance of alternative
   *
   * @param fileName filename
   * @param alt_id Alternative id
   */
  Performance getAlternativePerformance(std::string fileName,
                                        std::string alt_id);
  /**
   * Get all of alternative ids if they exists
   *
   * @param fileName filename
   *
   */
  std::vector<std::string> getAlternativeIds(std::string fileName);

  /**
   * Get all of criteria ids
   *
   * @param fileName filename
   *
   */
  std::vector<std::string> getCriteriaIds(std::string fileName);

  /**
   * Get weight for one specific criterion id
   *
   * @param fileName filename
   * @param crit_id Criterion id
   */
  Criterion getCriterion(std::string fileName, std::string crit_id);

  /**
   * Get alternative category assignment
   *
   * @param fileName filename
   * @param alt_id Criterion id
   */
  int getAlternativeAssignment(std::string fileName, std::string alt_id);

  /**
   * Get alternative category assignment
   *
   * @param fileName filename
   * @param crit_id Criterion id
   */
  std::vector<float> getCriterionCategoryLimits(std::string fileName,
                                                std::string crit_id);
};

#endif