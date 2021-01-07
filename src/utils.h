#ifndef UTILS_H
#define UTILS_H

#include "../extsrc/pugixml/src/pugixml.hpp"
#include <iostream>
#include <vector>

#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <time.h>
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

//   HOW TO USE
//   pugi::xml_document doc;
//   std::string path = "../data/" + fileName;
//   pugi::xml_parse_result result = doc.load_file(path.c_str());

//   simple_walker walker;
//   doc.traverse(walker);

inline bool fileExists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

/**
 * Random number generator between 0 and 1
 *
 * @return random  number
 */
inline float getRandomUniformNumber(bool changeSeed = 1) {
  if (changeSeed) {
    srand(time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
  } else {
    srand(0);
  }
  return (float)(rand() % 1000) / 1000;
}

/**
 * Random number generator between 1/2 and 1
 *
 * @return random  number
 */
inline float getRandomUniformNumberBis(bool changeSeed = 1) {
  if (changeSeed) {
    srand(time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    return (float)(rand() % 1000) / 2000 + 0.5;
  } else {
    return (float)(rand() % 1000) / 2000 + 0.5;
  }
}

inline std::vector<float> randomCriteriaLimits(int nbCategories,
                                               bool changeSeed = 1) {
  std::vector<float> critLimits;
  for (int i = 0; i < nbCategories; i++) {
    critLimits.push_back(getRandomUniformNumber(changeSeed));
  }
  sort(critLimits.begin(), critLimits.end());
  return critLimits;
}

#endif