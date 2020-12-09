#include "Performance.h"
#include <iostream>
#include <string>
#include <vector>

Performance::Performance(std::string id, Criteria criteria) {
  id_ = id;
  unsigned int len = criteria.size();
  std::pair<std::string, float> perf[len];
  for (int i = 0; i < len; i++) {
    perf[i] = std::make_pair(criteria[i].getId(), 0);
  }
  std::copy(perf, perf + len, perf_);
}

Performance::Performance(std::string id, Criteria criteria, float perf[]) {
  id_ = id;
  unsigned int len = criteria.size();
  std::pair<std::string, float> perf[len];
  for (int i = 0; i < len; i++) {
    perf[i] = std::make_pair(criteria[i].getId(), perf[i]);
  }
  std::copy(perf, perf + len, perf_);
}

Performance::Performance(const Performance &p) {
  id_ = p.getId();
  std::vector<std::pair<std::string, float>> *pperf = &p.getPerf();
  unsigned int len = pperf.size();
  std::pair<std::string, float> perf[len];
  for (int i = 0; i < len; i++) {
    perf[i] = std::make_pair(pperf[i].first, pperf[i].second);
  }
  std::copy(perf, perf + len, perf_);
}

Performance::~Performance() { delete perf_; }

std::string Performance::getId() const { return id_; }

void Performance::setId(std::string id) { id_ = id; }

std::vector<std::pair<std::string, float>> *Performance::getPerf() const {
  return perf_;
}

std::ostream &operator<<(std::ostream &out, const Performance &p) {
  out << "Performance(";
  for (auto cat : p.perf_) {
    out << "{ cat : " << cat.first << " perf : " << cat.second << " } ";
  }
  out << ")";
  return out;
}