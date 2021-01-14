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
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec);

template <typename T>
std::vector<T> subVector(std::vector<T> const &v, int m, int n);

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
//   std::string path = "../data/" + fileName;
//   pugi::xml_parse_result result = doc.load_file(path.c_str());

//   simple_walker walker;
//   doc.traverse(walker);

inline bool fileExists(const std::string &name);

/**
 * Random number generator between 0 and 1
 *
 * @return random  number
 */
inline float getRandomUniformNumber(bool changeSeed = 1);

/**
 * Random number generator between 1/2 and 1
 *
 * @return random  number
 */
inline float getRandomUniformNumberBis(bool changeSeed = 1);

/**
 * Random int number generator
 *
 * @param seed to initiate the generator
 * @param min min of generated number
 * @param max max of generated number
 *
 * @return random number
 */
inline int getRandomUniformInt(unsigned long int seed, int min, int max);

inline float getRandomUniformFloat(unsigned long int seed, float min,
                                   float max);

inline std::vector<float> randomCriteriaLimits(int nbCategories,
                                               bool changeSeed = 1);

#endif