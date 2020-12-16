#include "DataGenerator.h"
#include "Categories.h"
#include "Criteria.h"
#include "Perf.h"
#include "Performance.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void DataGenerator::DatasetGenerator(int nb_criteria, int nb_alternative,
                                     int nb_categories,
                                     std::string datasetName = "") {

  // Generate new XML document within memory
  pugi::xml_document doc;

  // Creating root
  auto root = doc.append_child("Root");
  pugi::xml_node dataset_node = doc.append_child("dataset");

  // Giving dataset name
  pugi::xml_node model_name = dataset_node.append_child("datasetName");
  std::string name = "dataset_alt" + std::to_string(nb_alternative) + "_crit" +
                     std::to_string(nb_criteria) + "_cat" +
                     std::to_string(nb_categories);

  if (datasetName == "") {
    model_name.append_child(pugi::node_pcdata).set_value(name.c_str());
  } else {
    model_name.append_child(pugi::node_pcdata).set_value(datasetName.c_str());
  }

  // Giving number of criterias in total
  pugi::xml_node criteria_node = dataset_node.append_child("criteria");

  criteria_node.append_child(pugi::node_pcdata)
      .set_value(std::to_string(nb_criteria).c_str());

  // Giving number of categories in total
  pugi::xml_node categories = dataset_node.append_child("categories");
  categories.append_child(pugi::node_pcdata)
      .set_value(std::to_string(nb_categories).c_str());

  // Giving number of alternatives in total
  pugi::xml_node alternatives = dataset_node.append_child("alternatives");
  alternatives.append_child(pugi::node_pcdata)
      .set_value(std::to_string(nb_alternative).c_str());

  // Creating a criteria object in order to create alternative performances
  Criteria criterias = Criteria(nb_criteria, "crit");

  for (int i = 0; i < nb_alternative; i++) {
    pugi::xml_node alternative_node = dataset_node.append_child("alternative");

    Performance alternative = Performance("alt" + std::to_string(i), criterias);

    // getting alternative id
    alternative_node.append_child(pugi::node_pcdata)
        .set_value(alternative.getId().c_str());

    for (int i = 0; i < alternative.getPerf().size(); i++) {
      // for each criteria give its correspondant performace
      pugi::xml_node alternative_criteria_node = alternative_node.append_child(
          alternative.getPerf()[i].getCrit().c_str());

      alternative_criteria_node.append_child(pugi::node_pcdata)
          .set_value(
              std::to_string(alternative.getPerf()[i].getValue()).c_str());
    }

    // Give the category assignemnt to the alternative
    pugi::xml_node alternative_assignment =
        alternative_node.append_child("assignment");

    alternative_assignment.append_child(pugi::node_pcdata)
        .set_value(
            "need to get category for it modify performance or other types");
  }

  if (datasetName == "") {
    std::string modelpath = "../data/" + name + ".xml";
  }
  std::string modelpath = "../data/" + datasetName;

  bool saveSucceeded = doc.save_file(modelpath.c_str(), PUGIXML_TEXT("  "));
  assert(saveSucceeded);
}

void DataGenerator::modelGenerator(int nb_criteria, int nb_categories,
                                   std::string modelName) {
  // Generate new XML document within memory
  pugi::xml_document doc;

  // Creating root
  auto root = doc.append_child("Root");
  pugi::xml_node dataset_node = doc.append_child("model");

  // Giving dataset name
  pugi::xml_node model_name = dataset_node.append_child("modelName");
  std::string name = "model_crit" + std::to_string(nb_criteria) + "_cat" +
                     std::to_string(nb_categories);

  if (modelName == "") {
    model_name.append_child(pugi::node_pcdata).set_value(name.c_str());
  } else {
    model_name.append_child(pugi::node_pcdata).set_value(modelName.c_str());
  }

  // Giving number of criterias in total
  pugi::xml_node criteria_node = dataset_node.append_child("criteria");

  criteria_node.append_child(pugi::node_pcdata)
      .set_value(std::to_string(nb_criteria).c_str());

  // Giving number of categories in total
  pugi::xml_node categories_node = dataset_node.append_child("categories");
  categories_node.append_child(pugi::node_pcdata)
      .set_value(std::to_string(nb_categories).c_str());

  // Giving lambda
  srand((unsigned)time(NULL));
  float lambda = (float)(rand() % 1000) / 2000 + 0.5;
  pugi::xml_node lambda_node = dataset_node.append_child("lambda");
  lambda_node.append_child(pugi::node_pcdata)
      .set_value(std::to_string(lambda).c_str());

  // Creating a criteria object in plot each criterion profile limit
  Criteria criteria = Criteria(nb_criteria, "crit");
  Categories categories = Categories(nb_categories);

  for (int i = 0; i < nb_criteria; i++) {
    pugi::xml_node criteria_node = dataset_node.append_child(
        criteria.getCriterionVect()[i].getId().c_str());

    for (int j = 0; j < nb_categories; j++) {
      pugi::xml_node profile_node =
          criteria_node.append_child(categories.getIdCategories()[j].c_str());

      profile_node.append_child(pugi::node_pcdata)
          .set_value("need to add performance of each profile for each crit");
    }

    // Give the weight assignmed to the criterion
    pugi::xml_node criteria_weight = criteria_node.append_child("weight");

    criteria_weight.append_child(pugi::node_pcdata)
        .set_value(
            std::to_string(criteria.getCriterionVect()[i].getWeight()).c_str());

    if (modelName == "") {
      std::string modelpath = "../data/" + name + ".xml";
    }
    std::string modelpath = "../data/" + modelName;

    bool saveSucceeded = doc.save_file(modelpath.c_str(), PUGIXML_TEXT("  "));
    assert(saveSucceeded);
  }
}

// void Generate::loadDataset(std::string fileName) {
//   pugi::xml_document doc;

//   pugi::xml_parse_result result = doc.load_file("../data" + fileName);

//   std::cout << "Load result: " << result.description()
//             << ", mesh name: " << doc.child("mesh").attribute("name").value()
//             << std::endl;
// }

int DataGenerator::getNumberOfCriteria(std::string fileName) const {
  pugi::xml_document doc;
  std::string path = "../data/" + fileName;
  if (!doc.load_file(path.c_str()))
    return -1;
  // if we have a model
  try {
    pugi::xml_node node_criteria = doc.child("model").child("criteria");

    std::stringstream strValue;
    strValue << node_criteria.child_value();

    unsigned int nb_crit;
    strValue >> nb_crit;

    return nb_crit;
  } catch (const std::exception &e) {
  }

  // if we have a dataset
  try {
    pugi::xml_node node_criteria = doc.child("dataset").child("criteria");

    std::stringstream strValue;
    strValue << node_criteria.child_value();

    unsigned int nb_crit;
    strValue >> nb_crit;

    return nb_crit;

  } catch (const std::exception &e) {
  }
  return 0;
}
