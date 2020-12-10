#include "PerformanceTable.h"
#include <iostream>
#include <string>
#include <vector>

PerformanceTable::PerformanceTable(std::vector<Performance> &perf_vect) {
  // deep copy
  for (int i = 0; i < perf_vect.size(); i++) {
    performance_table_.push_back(Performance(perf_vect[i]));
  }
}

PerformanceTable::PerformanceTable(std::string prefix, int nb_of_perfs,
                                   Criteria crits) {
  for (int i = 0; i < nb_of_perfs; i++) {
    performance_table_.push_back(Performance(prefix, crits));
  }
}

PerformanceTable::PerformanceTable(const PerformanceTable &perfs) {
  // deep copy
  for (int i = 0; i < perfs.performance_table_.size(); i++) {
    performance_table_.push_back(Performance(perfs.performance_table_[i]));
  }
}

PerformanceTable::~PerformanceTable() {}

std::ostream &operator<<(std::ostream &out, const PerformanceTable &perfs) {
  out << "PerformanceTable(";
  for (Performance perf : perfs.performance_table_) {
    out << perf << ", ";
  }
  out << ")";
  return out;
}

std::vector<Performance> PerformanceTable::getPerformanceTable() const {
  return performance_table_;
}