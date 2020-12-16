#include "PerformanceTable.h"
#include <iostream>
#include <string>
#include <vector>

PerformanceTable::PerformanceTable(std::vector<Performance> &perf_vect) {
  // deep copy
  for (int i = 0; i < perf_vect.size(); i++) {
    pt_.push_back(Performance(perf_vect[i]));
  }
}

PerformanceTable::PerformanceTable(std::string prefix, int nb_of_perfs,
                                   Criteria crits) {
  for (int i = 0; i < nb_of_perfs; i++) {
    pt_.push_back(Performance(prefix + std::to_string(i), crits));
  }
}

PerformanceTable::PerformanceTable(const PerformanceTable &perfs) {
  // deep copy
  for (int i = 0; i < perfs.pt_.size(); i++) {
    pt_.push_back(Performance(perfs.pt_[i]));
  }
}

PerformanceTable::~PerformanceTable() {}

std::ostream &operator<<(std::ostream &out, const PerformanceTable &perfs) {
  out << "PerformanceTable(";
  for (Performance perf : perfs.pt_) {
    out << perf << ", ";
  }
  out << ")";
  return out;
}

Performance PerformanceTable::operator[](std::string name) const {
  for (Performance p : pt_) {
    if (p.getId() == name) {
      return p;
    }
  }
  throw std::invalid_argument("Row not found in performance table");
}

std::vector<Performance> PerformanceTable::getPerformanceTable() const {
  return pt_;
}