#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.h
 * @brief utility functions used by our programs.
 *
 * All utility fonctions are referenced and implemented inside this file.
 */

#include "../extsrc/pugixml/src/pugixml.hpp"
#include "types/AlternativesPerformance.h"
#include "types/Profiles.h"
#include <matplot/matplot.h>

#include "types/Category.h"
#include "types/Criteria.h"
#include "types/Perf.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
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

/**
 * subVector get a sub-vector of a vector
 *
 * @param v vector from which we want the sub vector
 * @param m index of the beginning of subvector
 * @param n index of the end of subvector
 *
 * @return subvector
 */
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

/**
 * randomCategoriesLimits creates random categories limits given the number of
 * categories
 *
 * @param nbCategories number of categories (and of limits) to create
 * @param seed seed for random
 *
 * @return vector of float representing categories limits
 */
inline std::vector<float>
randomCategoriesLimits(int nbCategories, unsigned long int seed = time(NULL)) {
  std::vector<float> catLimits;
  for (int i = 0; i < nbCategories; i++) {
    catLimits.push_back(getRandomUniformFloat(seed));
  }
  sort(catLimits.begin(), catLimits.end());
  return catLimits;
}

/**
 * getCriterionIds get vector of criterions ids of a performance vector
 *
 * @param vectPerf performance vector
 *
 * @return vector of criterion ids
 */
inline std::vector<std::string> getCriterionIds(std::vector<Perf> vectPerf) {
  std::vector<std::string> criterionIds;
  for (auto p : vectPerf) {
    criterionIds.push_back(p.crit_);
  }
  return criterionIds;
};

/**
 * getPerfOfCrit get the performance for a given criterion in a performance
 * vector.
 *
 * @param critId criterion for which we want the perf
 *
 * @return Perf for given critId
 */
inline Perf getPerfOfCrit(std::vector<Perf> &vectPerf, std::string critId) {
  for (Perf p : vectPerf) {
    if (p.crit_ == critId) {
      return p;
    }
  }
  throw std::invalid_argument("No performance for given criterion found");
}

/**
 * getNameIds get the names of each perfomance in a perfomance vector
 *
 * @param vectPerf Performance vector from which we want the ids
 *
 * @return vector of names (ids)
 */
inline std::vector<std::string> getNameIds(std::vector<Perf> vectPerf) {
  std::vector<std::string> NameIds;
  for (auto p : vectPerf) {
    NameIds.push_back(p.name_);
  }
  return NameIds;
};

/**
 * createVectorPerf creates a vector of performances given the id, criteria and
 * values
 *
 * @param id of the performance
 * @param criteria criteria vector on which to associate the performances
 * @param given_perf performances values in the same order as criteria vect
 *
 * @return vect<Perf> performance vector
 */
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

/**
 * createVectorPerf creates a vector of performances with random values given
 * the id and criteria
 *
 * @param id of the performance
 * @param criteria criteria vector on which to associate the performances
 *
 * @return vect<Perf> performance vector
 */
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
 * Extract only performance values from a vector of Perf objects
 * @param p vector of Perf objects
 *
 */
inline std::vector<double> getPerfFromPerfVect(std::vector<Perf> &p) {
  std::vector<double> values;
  for (auto perf : p) {
    values.push_back(static_cast<double>(perf.value_));
  }
  return values;
}

/**
 * Check if a key is present in a unordered map
 * @param map unordered map of <int,int>
 *
 */
inline bool checkKey(std::unordered_map<int, int> &map, int key) {
  // Key is not present
  if (map.find(key) == map.end())
    return 0;

  return 1;
}
/**
 * Plots data distribution for a AlternativesPerformance object in order to
 * see if populations are separable. This is useful for ProfileInitialization
 * for example.
 *
 * @param ap AlternativesPerformance object
 *
 */
inline void plotGlobalData(AlternativesPerformance &ap) {
  matplot::cla();
  // get all of the category ranks
  std::unordered_map<std::string, Category> map =
      ap.getAlternativesAssignments();

  std::set<int> cat_set;
  for (auto pair : map) {
    cat_set.insert(pair.second.rank_);
  }
  // converting set to vector
  std::vector<int> ranks;
  ranks.assign(cat_set.begin(), cat_set.end());

  std::vector<std::vector<double>> performances;
  std::unordered_map<int, int> index;
  int nbCriteria = 0;
  for (std::vector<Perf> p : ap.getPerformanceTable()) {
    int rank = map[p[0].name_].rank_;
    if (nbCriteria == 0)
      nbCriteria = p.size();
    // if we have already seen this category yet
    if (checkKey(index, rank)) {
      std::vector<double> addPerf = getPerfFromPerfVect(p);
      performances[index[rank]].insert(performances[index[rank]].end(),
                                       addPerf.begin(), addPerf.end());
    } else {
      // create a new row for this new category and add Perf
      performances.push_back(getPerfFromPerfVect(p));
      // index of cat rank in the table
      index[rank] = performances.size() - 1;
    }
  }
  // creating reserve map
  std::unordered_map<int, int> reverse_index;
  for (auto pair : index) {
    reverse_index[pair.second] = pair.first;
  }

  int counter = 0;
  for (auto catPerf : performances) {
    int catRank = reverse_index[counter];
    if (catPerf.size() % nbCriteria != 0) {
      throw std::invalid_argument("Missing some criteria performance values in "
                                  "AlternativePerformance object");
    }
    int nbRepetition = catPerf.size() / nbCriteria - 1;
    std::vector<double> x(nbCriteria);
    std::iota(std::begin(x), std::end(x), 1);
    std::vector<double> x2 = x;
    for (int i = 0; i < nbRepetition; ++i) {
      x2.insert(std::end(x2), std::begin(x), std::end(x));
    }

    auto l = matplot::scatter(x2, catPerf, 10);
    l->marker_face(true);
    l->marker_style(matplot::line_spec::marker_style::diamond);
    l->display_name("Category " + std::to_string(catRank - 1));
    matplot::hold(matplot::on);
    counter++;
  }
  matplot::xlabel("Criteria");
  matplot::ylabel("Performance Value");
  matplot::title("Data Distribution Visualization");
  matplot::legend();
  matplot::show();
}

/**
 * Plots a visualization of a Profiles object. Profile object must be set in
 * "alt" mode in order to correctly plot.
 *
 * @param p Profiles object
 *
 */
inline void plotProfile(Profiles &p) {
  matplot::cla();
  // if (p.getMode() == "crit") {
  //   throw std::invalid_argument("Profile must be in alt mode");
  // }

  std::vector<std::vector<double>> prof_performances;
  for (std::vector<Perf> perf : p.getPerformanceTable()) {
    prof_performances.push_back(getPerfFromPerfVect(perf));
  }
  std::vector<double> x(prof_performances[0].size());
  std::iota(std::begin(x), std::end(x), 1);

  int counter = 0;
  for (auto proPerf : prof_performances) {

    auto l = matplot::plot(x, proPerf);
    l->display_name("b" + std::to_string(counter));
    matplot::hold(matplot::on);
    counter++;
  }
  matplot::xlabel("Criteria");
  matplot::ylabel("Performance Value");
  matplot::title("Profile Visualization");
  matplot::legend();
  matplot::show();
}

#endif