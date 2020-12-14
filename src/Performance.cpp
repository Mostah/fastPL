#include "Performance.h"
#include "Criteria.h"
#include "Perf.h"
#include <iostream>
#include <string>
#include <vector>

Performance::Performance(std::string id, Criteria &criteria) {
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    performance_.push_back(Perf(id, criterion_vect[i].getId(), 0));
  }
}

Performance::Performance(std::string id, Criteria &criteria,
                         std::vector<float> &given_perf) {
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    performance_.push_back(Perf(id, criterion_vect[i].getId(), given_perf[i]));
  }
}

Performance::Performance(std::vector<Perf> &perf) {
  for (int i = 0; i < perf.size(); i++) {
    performance_.push_back(
        Perf(perf[i].getName(), perf[i].getCrit(), perf[i].getValue()));
  }
}

Performance::Performance(const Performance &p) {
  std::vector<Perf> perf = p.getPerf();
  for (int i = 0; i < perf.size(); i++) {
    performance_.push_back(
        Perf(perf[i].getName(), perf[i].getCrit(), perf[i].getValue()));
  }
}

Performance::~Performance() {}

std::vector<Perf> Performance::getPerf() const { return performance_; }

std::ostream &operator<<(std::ostream &out, const Performance &p) {
  out << "Performance(";
  for (Perf perf : p.performance_) {
    out << perf << ", ";
  }
  out << ")";
  return out;
}