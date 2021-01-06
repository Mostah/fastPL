#ifndef UTILS_H
#define UTILS_H

#include <iostream>
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

#endif