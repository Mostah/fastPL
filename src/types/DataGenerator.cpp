#include "../../include/types/DataGenerator.h"
#include "../../include/app.h"
#include "../../include/types/Categories.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/Criterion.h"
#include "../../include/types/Perf.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <unordered_map>

DataGenerator::DataGenerator(Config &config) : conf(config) {}

void DataGenerator::datasetGenerator(int nb_criteria, int nb_alternative,
                                     int nb_categories, std::string datasetName,
                                     bool overwrite, unsigned long int seed) {

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

    std::vector<Perf> alternative;
    alternative =
        createVectorPerfWithNoPerf("alt" + std::to_string(i), criterias);

    // getting alternative id
    alternative_node.append_child(pugi::node_pcdata)
        .set_value(alternative[0].getName().c_str());

    for (Perf perf : alternative) {
      // for each criteria give its correspondant performace
      pugi::xml_node alternative_criteria_node =
          alternative_node.append_child(perf.getCrit().c_str());

      alternative_criteria_node.append_child(pugi::node_pcdata)
          .set_value(std::to_string(perf.getValue()).c_str());
    }

    // Give the category assignemnt to the alternative
    pugi::xml_node alternative_assignment =
        alternative_node.append_child("assignment");

    alternative_assignment.append_child(pugi::node_pcdata)
        .set_value(
            "need to get category for it modify performance or other types");
  }

  std::string modelpath;
  if (datasetName == "") {
    modelpath = conf.data_dir + name + ".xml";
  } else {
    modelpath = conf.data_dir + datasetName;
  }

  if (fileExists(modelpath) && !overwrite) {
    throw std::invalid_argument("Such a default xml generate (or not) filename "
                                "already exists and you chose "
                                "not to overwrite it");
  }
  bool saveSucceeded = doc.save_file(modelpath.c_str(), PUGIXML_TEXT("  "));
  if (!saveSucceeded) {
    throw std::invalid_argument("Cannot save xml file...");
  }
}

void DataGenerator::modelGenerator(int nb_criteria, int nb_categories,
                                   std::string modelName, bool overwrite,
                                   unsigned long int seed) {
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
  // Lambda is a value between 0.5 and 1
  float lambda = getRandomUniformFloat(seed, 0.5, 1);

  pugi::xml_node lambda_node = dataset_node.append_child("lambda");
  lambda_node.append_child(pugi::node_pcdata)
      .set_value(std::to_string(lambda).c_str());

  // Creating a criteria object in plot each criterion profile limit
  Criteria criteria = Criteria(nb_criteria, "crit");
  criteria.generateRandomCriteriaWeights(seed);
  Categories categories = Categories(nb_categories);

  for (int i = 0; i < nb_criteria; i++) {
    pugi::xml_node criteria_node = dataset_node.append_child(
        criteria.getCriterionVect()[i].getId().c_str());

    // Generate random Criteria Limits
    std::vector<float> critLimit = randomCategoriesLimits(nb_categories, 0);
    for (int j = 0; j < nb_categories; j++) {
      pugi::xml_node profile_node =
          criteria_node.append_child(categories.getIdCategories()[j].c_str());
      profile_node.append_child(pugi::node_pcdata)
          .set_value(std::to_string(critLimit[j]).c_str());
    }

    // Give the weight assignmed to the criterion
    pugi::xml_node criteria_weight = criteria_node.append_child("weight");

    criteria_weight.append_child(pugi::node_pcdata)
        .set_value(
            std::to_string(criteria.getCriterionVect()[i].getWeight()).c_str());

    // Add direction needed for the criterion
    pugi::xml_node direction_node = criteria_node.append_child("direction");

    direction_node.append_child(pugi::node_pcdata)
        .set_value(std::to_string(criteria.getCriterionVect()[i].getDirection())
                       .c_str());
  }
  std::string modelpath;
  if (modelName == "") {
    modelpath = conf.data_dir + name + ".xml";
  } else {
    modelpath = conf.data_dir + modelName;
  }

  if (fileExists(modelpath) && !overwrite) {
    throw std::invalid_argument("Such a default xml generate (or not) filename "
                                "already exists and you chose "
                                "not to overwrite it");
  }

  bool saveSucceeded = doc.save_file(modelpath.c_str(), PUGIXML_TEXT("  "));
  if (!saveSucceeded) {
    throw std::invalid_argument("Cannot save xml file...");
  }
}

std::tuple<float, Criteria, PerformanceTable>
DataGenerator::loadModel(std::string fileName) {
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) != "model") {
    // if we have a model xml
    throw std::invalid_argument(
        "Cannot find any lambda and profile performances in xml file, "
        "most likely have a dataset xml file");
  } else {

    // Getting lambda from model
    float lambda = DataGenerator::getThresholdValue(fileName);
    std::vector<Criterion> criteria_vec;
    std::vector<std::string> criteriaIds =
        DataGenerator::getCriteriaIds(fileName);
    std::vector<std::vector<float>> catLimits;
    for (std::vector<std::string>::iterator it = criteriaIds.begin(),
                                            end = criteriaIds.end();
         it != end; ++it) {
      criteria_vec.push_back(DataGenerator::getCriterion(fileName, *it));
      catLimits.push_back(
          DataGenerator::getCriterionCategoryLimits(fileName, *it));
    }

    Criteria criteria = Criteria(criteria_vec);
    int nbCriteria = catLimits.size();
    int nbFictionalAlternatives = catLimits[0].size();
    std::vector<std::vector<float>> fictPerformances;
    for (int i = 0; i < nbFictionalAlternatives; i++) {
      std::vector<float> tmp;
      for (int j = 0; j < nbCriteria; j++) {
        tmp.push_back(catLimits[j][i]);
      }
      fictPerformances.push_back(tmp);
    }

    std::vector<std::vector<Perf>> vecPerformance;
    for (int i = 0; i < nbFictionalAlternatives; i++) {
      std::vector<Perf> p;
      for (int j = 0; j < fictPerformances[i].size(); j++) {
        p.push_back(Perf("prof" + std::to_string(i), criteria[j].getId(),
                         fictPerformances[i][j]));
      }
      vecPerformance.push_back(p);
    }
    PerformanceTable perftab = PerformanceTable(vecPerformance);
    return std::make_tuple(lambda, criteria, perftab);
  }
}

void DataGenerator::saveModel(std::string fileName, float lambda,
                              Criteria criteria, PerformanceTable pt,
                              bool overwrite, std::string modelName) {

  int nb_categories = pt.getPerformanceTable().size();
  int nb_criteria = criteria.getCriterionVect().size();

  if (nb_categories != pt.getPerformanceTable()[0].size()) {
    throw std::invalid_argument(
        " Number of criteria and the number and the length of the "
        "performance "
        "of the fictive alternative (ie profile performance) does not match");
  }
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
  pugi::xml_node lambda_node = dataset_node.append_child("lambda");
  lambda_node.append_child(pugi::node_pcdata)
      .set_value(std::to_string(lambda).c_str());

  for (int i = 0; i < nb_criteria; i++) {
    pugi::xml_node criteria_node = dataset_node.append_child(
        criteria.getCriterionVect()[i].getId().c_str());

    // Giving category limits from pt data
    for (int j = 0; j < nb_categories; j++) {
      std::string cat_id = "cat" + std::to_string(j);
      pugi::xml_node profile_node = criteria_node.append_child(cat_id.c_str());
      profile_node.append_child(pugi::node_pcdata)
          .set_value(
              std::to_string(
                  pt.getPerformanceTable()[nb_categories - j - 1][i].getValue())
                  .c_str());
    }

    // Give the weight assignmed to the criterion
    pugi::xml_node criteria_weight = criteria_node.append_child("weight");

    criteria_weight.append_child(pugi::node_pcdata)
        .set_value(
            std::to_string(criteria.getCriterionVect()[i].getWeight()).c_str());

    // Add direction needed for the criterion
    pugi::xml_node direction_node = criteria_node.append_child("direction");

    direction_node.append_child(pugi::node_pcdata)
        .set_value(std::to_string(criteria.getCriterionVect()[i].getDirection())
                       .c_str());
  }
  std::string modelpath = conf.data_dir + fileName;
  if (fileExists(modelpath) && !overwrite) {
    throw std::invalid_argument("Such a default xml generate (or not) filename "
                                "already exists and you chose "
                                "not to overwrite it");
  }

  bool saveSucceeded = doc.save_file(modelpath.c_str(), PUGIXML_TEXT("  "));
  if (!saveSucceeded) {
    throw std::invalid_argument("Cannot save xml file...");
  }
}

AlternativesPerformance DataGenerator::loadDataset(std::string fileName) {
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);
  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    throw std::invalid_argument("Cannot find any alternatives in xml file, "
                                "most likely have a xml model file");
  }
  // creating type blocks to return Performance object
  std::unordered_map<std::string, Category> altAssignments;
  int nb_criteria = DataGenerator::getNumberOfCriteria(fileName);
  Criteria criteria = Criteria(nb_criteria, "crit");
  std::vector<std::vector<Perf>> vecPerformances;

  pugi::xml_node node_dataset = doc.child("dataset");
  for (pugi::xml_node_iterator it = node_dataset.begin();
       it != node_dataset.end(); ++it) {
    std::vector<float> altPerf;

    if (strcmp(it->name(), "alternative") == 0) {

      pugi::xml_node alternative_node = node_dataset.child(it->name());
      std::string altId = it->child_value();

      for (pugi::xml_node_iterator it = alternative_node.begin();
           it != alternative_node.end(); ++it) {

        if (strcmp(it->name(), "assignment") != 0 ||
            strcmp(it->name(), "") != 0) {

          float perf = atof(it->child_value());
          altPerf.push_back(perf);

        } else if (strcmp(it->name(), "assignment") == 0) {

          float rank = atof(it->child_value());
          altAssignments.insert(
              {altId, Category("cat" + std::to_string(rank), rank)});
        }
      }
      vecPerformances.push_back(createVectorPerf(altId, criteria, altPerf));
    }
  }
  return AlternativesPerformance(PerformanceTable(vecPerformances),
                                 altAssignments);
}

void DataGenerator::saveDataset(std::string fileName,
                                AlternativesPerformance altPerf,
                                int nb_categories, bool overwrite,
                                std::string datasetName) {

  if (altPerf.getMode() != "alt") {
    throw std::invalid_argument("Performance table must be in alt mode");
  }

  int nb_alternatives = altPerf.getPerformanceTable().size();
  int nb_criteria = altPerf.getPerformanceTable()[0].size();

  // Generate new XML document within memory
  pugi::xml_document doc;

  // Creating root
  auto root = doc.append_child("Root");
  pugi::xml_node dataset_node = doc.append_child("dataset");

  // Giving dataset name
  pugi::xml_node model_name = dataset_node.append_child("datasetName");
  std::string name = "dataset_alt" + std::to_string(nb_alternatives) + "_crit" +
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
      .set_value(std::to_string(nb_alternatives).c_str());

  // looping over alternatives
  for (std::vector<Perf> p : altPerf.getPerformanceTable()) {
    pugi::xml_node alternative_node = dataset_node.append_child("alternative");
    // getting alternative id
    alternative_node.append_child(pugi::node_pcdata)
        .set_value(p[0].getName().c_str());

    // looping over performance values of specific alternative
    for (Perf perf : p) {
      // for each criteria give its correspondant performace
      pugi::xml_node alternative_criteria_node =
          alternative_node.append_child(perf.getCrit().c_str());

      alternative_criteria_node.append_child(pugi::node_pcdata)
          .set_value(std::to_string(perf.getValue()).c_str());
    }

    // Get the category assignemnt of the alternative from altPerf
    pugi::xml_node alternative_assignment =
        alternative_node.append_child("assignment");

    alternative_assignment.append_child(pugi::node_pcdata)
        .set_value(
            std::to_string(altPerf.getAlternativeAssignment(p[0].getName())
                               .getCategoryRank())
                .c_str());
  }

  std::string modelpath;
  if (datasetName == "") {
    modelpath = conf.data_dir + name + ".xml";
  } else {
    modelpath = conf.data_dir + datasetName;
  }

  if (fileExists(modelpath) && !overwrite) {
    throw std::invalid_argument("Such a default xml generate (or not) filename "
                                "already exists and you chose "
                                "not to overwrite it");
  }
  bool saveSucceeded = doc.save_file(modelpath.c_str(), PUGIXML_TEXT("  "));
  if (!saveSucceeded) {
    throw std::invalid_argument("Cannot save xml file...");
  }
}

pugi::xml_document DataGenerator::openXmlFile(std::string fileName) {
  pugi::xml_document doc;
  std::string path = conf.data_dir + fileName;
  if (!doc.load_file(path.c_str()))
    throw std::invalid_argument("Cannot open xml file, please check path");
  return doc;
}

std::string DataGenerator::getXmlFileType(std::string fileName) {

  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);
  pugi::xml_node model_node = doc.child("model").child("modelName");
  pugi::xml_node dataset_node = doc.child("dataset").child("datasetName");

  if (!*model_node.child_value() && !*dataset_node.child_value()) {
    // if both pointers are null there we have a wrong xml structure
    throw std::invalid_argument("xml file structure is not right");
  }
  if (!*model_node.child_value()) {
    // if we have a dataset xml
    return "dataset";
  }
  // if we have a model xml
  return "model";
}

int DataGenerator::getNumberOfCriteria(std::string fileName) {

  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);
  pugi::xml_node node_criteria;
  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    node_criteria = doc.child("model").child("criteria");
  } else {
    // if we have a dataset xml
    node_criteria = doc.child("dataset").child("criteria");
  }

  std::stringstream strValue;
  int nb_crit;
  strValue << node_criteria.child_value();
  strValue >> nb_crit;
  return nb_crit;
}

int DataGenerator::getNumberOfCategories(std::string fileName) {

  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);
  pugi::xml_node node_cat;
  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    node_cat = doc.child("model").child("categories");
  } else {
    // if we have a dataset xml
    node_cat = doc.child("dataset").child("categories");
  }

  std::stringstream strValue;
  int nb_categories;
  strValue << node_cat.child_value();
  strValue >> nb_categories;
  return nb_categories;
}

float DataGenerator::getThresholdValue(std::string fileName) {
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    pugi::xml_node lambda_node = doc.child("model").child("lambda");
    float lambda = atof(lambda_node.child_value());
    return lambda;
  } else {
    // if we have a dataset xml
    throw std::invalid_argument("Cannot find any threshold in xml file, "
                                "most likely have a xml dataset file");
  }
}

int DataGenerator::getNumberOfAlternatives(std::string fileName) {
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    throw std::invalid_argument(
        "Cannot find the number of alternatives in xml file, "
        "most likely have a xml model file");
  } else {
    // if we have a dataset xml
    pugi::xml_node node_alternatives =
        doc.child("dataset").child("alternatives");
    std::stringstream strValue;
    int nb_alt;
    strValue << node_alternatives.child_value();
    strValue >> nb_alt;
    return nb_alt;
  }
}

std::vector<Perf> DataGenerator::getAlternativePerformance(std::string fileName,
                                                           std::string alt_id) {

  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    throw std::invalid_argument("Cannot find any alternatives in xml file, "
                                "most likely have a xml model file");
  } else {

    // creating type blocks to return Performance object
    int nb_criteria = DataGenerator::getNumberOfCriteria(fileName);
    Criteria criteria = Criteria(nb_criteria, "crit");
    std::vector<float> alt_perf;

    pugi::xml_node node_dataset = doc.child("dataset");
    for (pugi::xml_node_iterator it = node_dataset.begin();
         it != node_dataset.end(); ++it) {

      if (it->child_value() == alt_id) {
        pugi::xml_node alternative_node = node_dataset.child(it->name());

        for (pugi::xml_node_iterator al_it = alternative_node.begin();
             al_it != alternative_node.end(); ++al_it) {

          if (strcmp(al_it->name(), "assignment") != 0 &&
              strcmp(al_it->name(), "") != 0) {
            float perf = atof(al_it->child_value());
            alt_perf.push_back(perf);
          }
        }
        break;
      }
    }
    if (alt_perf.empty()) {
      throw std::invalid_argument("Cannot find performance associated to the "
                                  "alternative identified by "
                                  "alt_id in xml file.");
    }
    return createVectorPerf(alt_id, criteria, alt_perf);
  }
}

std::vector<std::string>
DataGenerator::getAlternativeIds(std::string fileName) {
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    throw std::invalid_argument("Cannot find any alternatives in xml file, "
                                "most likely have a xml model file");
  } else {
    // creating vector object to store alternative ids
    std::vector<std::string> alt_ids;

    pugi::xml_node node_dataset = doc.child("dataset");
    for (pugi::xml_node_iterator it = node_dataset.begin();
         it != node_dataset.end(); ++it) {
      if (strcmp(it->name(), "alternative") == 0) {
        const char *s = it->child_value();
        std::string str(s);
        alt_ids.push_back(s);
      }
    }
    return alt_ids;
  }
}

std::vector<std::string> DataGenerator::getCriteriaIds(std::string fileName) {
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  // creating vector object to store criteria ids
  std::vector<std::string> crit_ids;

  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    // going from root ie model then to its first child modelName and
    // passing to the 5th child ie first criterion
    pugi::xml_node node_model = doc.child("model");

    for (pugi::xml_node_iterator it = node_model.begin();
         it != node_model.end(); ++it) {
      if (strcmp(it->name(), "modelName") != 0 &&
          strcmp(it->name(), "criteria") != 0 &&
          strcmp(it->name(), "categories") != 0 &&
          strcmp(it->name(), "lambda") != 0) {
        const char *s = it->name();
        std::string str(s);
        crit_ids.push_back(s);
      }
    }
  } else {
    // if we have a dataset xml
    pugi::xml_node node_dataset = doc.child("dataset").child("alternative");
    for (pugi::xml_node_iterator it = node_dataset.begin();
         it != node_dataset.end(); ++it) {
      if (strcmp(it->name(), "") != 0 & strcmp(it->name(), "assignment") != 0) {
        const char *s = it->name();
        std::string str(s);
        crit_ids.push_back(s);
      }
    }
  }
  return crit_ids;
}

Criterion DataGenerator::getCriterion(std::string fileName,
                                      std::string crit_id) {
  float weight = -1;
  float direction = 0;
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "dataset") {
    // if we have a model xml
    throw std::invalid_argument("Cannot find the criterion associated to "
                                "its weights in xml file, "
                                "most likely have a xml model file");
  } else {
    // if we have a dataset xml
    pugi::xml_node node_model = doc.child("model");

    for (pugi::xml_node_iterator it = node_model.begin();
         it != node_model.end(); ++it) {

      if (it->name() == crit_id) {
        pugi::xml_node criterion_node = node_model.child(it->name());

        for (pugi::xml_node_iterator al_it = criterion_node.begin();
             al_it != criterion_node.end(); ++al_it) {

          if (strcmp(al_it->name(), "weight") == 0) {
            weight = atof(al_it->child_value());
          }
          if (strcmp(al_it->name(), "direction") == 0) {
            direction = atof(al_it->child_value());
          }
        }
        break;
      }
    }
  }
  if (weight == -1 || direction == 0) {
    throw std::invalid_argument(
        "Cannot find Criterion associated to crit_id in xml file.");
  }
  return Criterion(crit_id, direction, weight);
}

int DataGenerator::getAlternativeAssignment(std::string fileName,
                                            std::string alt_id) {

  int assignment = -1;
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "model") {
    // if we have a model xml
    throw std::invalid_argument("Cannot find category assignment associated to "
                                "alternative in xml file, "
                                "most likely have a xml model file");
  } else {

    pugi::xml_node node_dataset = doc.child("dataset");
    for (pugi::xml_node_iterator it = node_dataset.begin();
         it != node_dataset.end(); ++it) {

      if (it->child_value() == alt_id) {
        pugi::xml_node alternative_node = node_dataset.child(it->name());

        for (pugi::xml_node_iterator al_it = alternative_node.begin();
             al_it != alternative_node.end(); ++al_it) {

          if (strcmp(al_it->name(), "assignment") == 0) {
            // std::cout << "Success" << al_it->child_value();
            // need to add when NEW TYPE WILL BE SET
            assignment = 0;
          }
        }
      }
    }
  }
  if (assignment == -1) {
    throw std::invalid_argument("Cannot find category assignment "
                                "associated to alt_id in xml file.");
  }
  return assignment;
}

std::vector<float>
DataGenerator::getCriterionCategoryLimits(std::string fileName,
                                          std::string crit_id) {

  std::vector<float> categoryLimits;
  pugi::xml_document doc = DataGenerator::openXmlFile(fileName);

  if (DataGenerator::getXmlFileType(fileName) == "dataset") {
    // if we have a model xml

    throw std::invalid_argument("Cannot find category limits associated to "
                                "criterion in xml file, most "
                                "likely have a xml dataset file");

  } else {
    // if we have a dataset xml
    pugi::xml_node node_model = doc.child("model");

    for (pugi::xml_node_iterator it = node_model.begin();
         it != node_model.end(); ++it) {
      if (it->name() == crit_id) {
        pugi::xml_node criterion_node = node_model.child(it->name());

        for (pugi::xml_node_iterator al_it = criterion_node.begin();
             al_it != criterion_node.end(); ++al_it) {

          if (strcmp(al_it->name(), "weight") != 0 &&
              strcmp(al_it->name(), "direction") != 0) {
            float limit = atof(al_it->child_value());
            categoryLimits.push_back(limit);
          }
        }
        break;
      }
    }
  }
  if (categoryLimits.empty()) {
    throw std::invalid_argument("Cannot find criterion category limit "
                                "associated to crit_id in xml file.");
  }
  return categoryLimits;
}
