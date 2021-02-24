#ifndef UTILS_H
#define UTILS_H

//#include "../extsrc/matplotlib-cpp/matplotlibcpp.h"
#include "../extsrc/pugixml/src/pugixml.hpp"
#include "types/AlternativesPerformance.h"

#include "types/Category.h"
#include "types/Criteria.h"
#include "types/Perf.h"
#include <iostream>
#include <random>
#include <vector>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <time.h>
#include <typeinfo>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * Overloading << operator for std::vector object
 *
 * @param out ostream
 * @param vec std::vector object
 *
 */
template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec) {
  out << "[";
  for (std::size_t i = 0; i < vec.size(); i++) {
    out << vec[i] << (i == vec.size() - 1 ? "" : ",");
  }
  out << "]";
  return out;
}

template <typename T>
std::vector<T> subVector(std::vector<T> const &v, int m, int n) {
  auto first = v.begin() + m;
  auto last = v.begin() + n + 1;
  std::vector<T> vector(first, last);
  return vector;
}

/**
 * Pugy XML Tree walker viewer
 *
 * @param node xml_node
 *
 */
struct simple_walker : pugi::xml_tree_walker {
  virtual bool for_each(pugi::xml_node &node) {
    for (int i = 0; i < depth(); ++i)
      std::cout << "  "; // indentation

    std::cout << node.type() << ": name='" << node.name() << "', value='"
              << node.value() << "'\n";

    return true; // continue traversal
  }
};
//   ^^^^^^^^^^^^ HOW TO USE ^^^^^^^^^^^^
//   pugi::xml_document doc;
//   std::string path = data_dir + fileName;
//   pugi::xml_parse_result result = doc.load_file(path.c_str());

//   simple_walker walker;
//   doc.traverse(walker);

inline bool fileExists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

/**
 * Random int number generator
 *
 * @param seed to initiate the generator, default 0
 * @param min min of generated number, default 0
 * @param max max of generated number, default 100
 *
 * @return random int number between min and max
 */
inline int getRandomUniformInt(unsigned long int seed = 0, int min = 0,
                               int max = 100) {
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

/**
 * Random float number generator
 *
 * @param seed to initiate the generator, default 0
 * @param min min of generated number, default 0
 * @param max max of generated number, default 1
 *
 * @return random float number between min and max
 */
inline float getRandomUniformFloat(unsigned long int seed = 0, float min = 0,
                                   float max = 1) {

  std::mt19937 gen(seed);
  std::uniform_real_distribution<> dis(min, max);
  return dis(gen);
}

inline std::vector<float>
randomCategoriesLimits(int nbCategories, unsigned long int seed = time(NULL)) {
  std::vector<float> catLimits;
  for (int i = 0; i < nbCategories; i++) {
    catLimits.push_back(getRandomUniformFloat(seed));
  }
  sort(catLimits.begin(), catLimits.end());
  return catLimits;
}

inline std::vector<std::string> getCriterionIds(std::vector<Perf> vectPerf) {
  std::vector<std::string> criterionIds;
  for (auto p : vectPerf) {
    criterionIds.push_back(p.getCrit());
  }
  return criterionIds;
};

inline std::vector<Perf> createVectorPerf(std::string id, Criteria &criteria,
                                          std::vector<float> &given_perf) {

  if (criteria.getCriterionVect().size() != given_perf.size()) {
    throw std::invalid_argument(
        "You must have the same number of performance value and criterias");
  }
  std::vector<Perf> vp;
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    vp.push_back(Perf(id, criterion_vect[i].getId(), given_perf[i]));
  }
  return vp;
}

inline std::vector<Perf> createVectorPerfWithNoPerf(std::string id,
                                                    Criteria &criteria) {
  std::vector<Perf> vp;
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    vp.push_back(Perf(id, criterion_vect[i].getId(), 0));
  }
  return vp;
}

/**
 * Plots data distribution for a AlternativesPerformance object in order to see
 * if populations are separable. This is useful for ProfileInitialization for
 * example
 *
 * @param ap AlternativesPerformance object
 *
 */
inline void plotData(AlternativesPerformance &ap) {

  std::unordered_map<std::string, Category> map =
      ap.getAlternativesAssignments();
  // needs to be filled with matplotliblibrary linked
}
#endif