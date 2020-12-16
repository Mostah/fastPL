#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

// will be using pugiXML for parsing and generating
// git clone https://github.com/zeux/pugixml.git
// to fix error You can fix this by editing the file called pugiconfig.hpp
// and uncommenting the line here: Uncomment this to switch to header-only
// version #define PUGIXML_HEADER_ONLY

#include "../pugixml/src/pugixml.hpp"
#include "AtomicMCDAObject.h"
#include <iostream>

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
   *
   * @return a saved file of the name datasetName.xml in data/ or default
   * generated name
   */
  void DatasetGenerator(int nb_criteria, int nb_alternative, int nb_categories,
                        std::string datasetName);

  /**
   * Generate a random model and puts it in a xml file
   *
   * @param nb_criteria number of criteria
   * @param nb_categories number of categories
   * @param modelName name of that model
   *
   * @return a saved file of the name nametName.xml in data/ or default
   * generated name
   */
  void modelGenerator(int nb_criteria, int nb_categories,
                      std::string modelName);

  /**
   * Get data from xml file
   *
   * @param fileName filename
   *
   * @return a type for model usage
   *
   */
  void loadDataset(std::string fileName);

  // void saveDataset(new type, std::string datasetName);

  /**
   * Get number of criterias found in xml file
   *
   * @param fileName filename
   *
   */
  int getNumberOfCriteria(std::string fileName) const;
};

#endif