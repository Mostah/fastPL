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
// struct simple_walker : pugi::xml_tree_walker {
//   virtual bool for_each(pugi::xml_node &node) {
//     for (int i = 0; i < depth(); ++i)
//       std::cout << "  "; // indentation

//     std::cout << node.type() << ": name='" << node.name() << "', value='"
//               << node.value() << "'\n";

//     return true; // continue traversal
//   }
// };
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
 * @param min min of generated number
 * @param max max of generated number
 * @param seed to initiate the generator
 *
 * @return random int number between min and max
 */
inline int getRandomUniformInt(int min, int max, unsigned long int seed = 0) {
  srand(seed);
  return min + rand() % (max - min);
}

/**
 * Random float number generator
 *
 * @param min min of generated number
 * @param max max of generated number
 * @param seed to initiate the generator
 *
 * @return random float number between min and max
 */
inline float getRandomUniformFloat(float min = 0, float max = 1,
                                   unsigned long int seed = 0) {
  srand(seed);
  return min + (((float)rand()) / (float)RAND_MAX) * (max - min);
}

inline std::vector<float> randomCategoriesLimits(int nbCategories,
                                                 unsigned long int seed = 0) {
  std::vector<float> catLimits;
  for (int i = 0; i < nbCategories; i++) {
    catLimits.push_back(getRandomUniformFloat(0, 1, seed));
  }
  sort(catLimits.begin(), catLimits.end());
  return catLimits;
}

#endif