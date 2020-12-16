#include "PerformanceTable.h"
#include <iostream>
#include <string>
#include <vector>

PerformanceTable::PerformanceTable(std::vector<Performance> &perf_vect) {
  if (perf_vect.size() == 0) {
    throw std::invalid_argument("The vector must contain performances.");
  }
  std::vector<std::string> perf_id_vect;
  std::vector<std::string> crit_vect = perf_vect[0].getCriteria();

  for (int i = 0; i < perf_vect.size(); i++) {
    // ensure there is no performance with dupplicated name
    if (std::find(perf_id_vect.begin(), perf_id_vect.end(),
                  perf_vect[i].getId()) != perf_id_vect.end()) {
      throw std::invalid_argument("Each performance must have different ids.");
    }
    perf_id_vect.push_back(perf_vect[i].getId());

    // ensure all the performance are based on the same set of criterion
    if (perf_vect[i].getCriteria() != crit_vect) {
      throw std::invalid_argument("Each performance must be based on the same "
                                  "set of criterion, in the same order.");
    }
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