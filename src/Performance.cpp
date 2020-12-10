#include "Performance.h"
#include "Criteria.h"
#include <iostream>
#include <string>
#include <vector>

Performance::Performance(std::string id, Criteria &criteria) {
  id_ = id;
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    perf_.push_back(std::make_pair(criterion_vect[i].getId(), 0));
  }
}

Performance::Performance(std::string id, Criteria &criteria,
                         std::vector<float> &given_perf) {
  id_ = id;
  std::vector<Criterion> criterion_vect = criteria.getCriterionVect();
  for (int i = 0; i < criterion_vect.size(); i++) {
    perf_.push_back(std::make_pair(criterion_vect[i].getId(), given_perf[i]));
  }
}

Performance::Performance(const Performance &p) {
  id_ = p.getId();
  std::vector<std::pair<std::string, float>> pperf = p.getPerf();
  for (int i = 0; i < pperf.size(); i++) {
    perf_.push_back(std::make_pair(pperf[i].first, pperf[i].second));
  }
}

Performance::~Performance() {}

std::string Performance::getId() const { return id_; }

void Performance::setId(std::string id) { id_ = id; }

std::vector<std::pair<std::string, float>> Performance::getPerf() const {
  return perf_;
}

std::ostream &operator<<(std::ostream &out, const Performance &p) {
  out << "Performance(";
  for (std::pair<std::string, float> cat : p.perf_) {
    out << "{ cat : " << cat.first << ", perf : " << cat.second << " }, ";
  }
  out << ")";
  return out;
}