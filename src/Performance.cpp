#include "../include/Performance.h"
#include "../include/Criteria.h"
#include "../include/Perf.h"
#include <iostream>
#include <string>
#include <vector>

Performance::Performance(Criteria &criteria, std::string id) {
  id_ = id;
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    performance_.push_back(Perf(id, criterion_vect[i].getId(), 0));
  }
}

Performance::Performance(Criteria &criteria, std::vector<float> &given_perf,
                         std::string id) {
  id_ = id;
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    performance_.push_back(Perf(id, criterion_vect[i].getId(), given_perf[i]));
  }
}

Performance::Performance(std::vector<Perf> &perf) {
  id_ = perf[0].getName();
  for (int i = 0; i < perf.size(); i++) {
    // a Performance should have the same name for every Perf object
    if (perf[i].getName() != id_) {
      throw std::invalid_argument(
          "All Perf in the vector must have the same name");
    }
    performance_.push_back(Perf(id_, perf[i].getCrit(), perf[i].getValue()));
  }
}

Performance::Performance(const Performance &p) {
  std::vector<Perf> perf = p.getPerf();
  id_ = perf[0].getName();
  for (int i = 0; i < perf.size(); i++) {
    performance_.push_back(
        Perf(perf[i].getName(), perf[i].getCrit(), perf[i].getValue()));
  }
}

Performance::~Performance() {}

std::vector<Perf> Performance::getPerf() const { return performance_; }

std::string Performance::getId() const { return id_; }

void Performance::setId(std::string id) { id_ = id; }

std::ostream &operator<<(std::ostream &out, const Performance &p) {
  out << "Performance(";
  for (Perf perf : p.performance_) {
    out << perf << ", ";
  }
  out << ")";
  return out;
}

Perf Performance::operator[](std::string criterion) const {
  for (Perf perf : performance_) {
    if (perf.getCrit() == criterion) {
      return perf;
    }
  }
  throw std::invalid_argument("criterion not found in performance vector");
}

std::vector<std::string> Performance::getCriterionIds() {
  std::vector<std::string> crit_vect;
  for (Perf p : performance_) {
    crit_vect.push_back(p.getCrit());
  }
  return crit_vect;
}