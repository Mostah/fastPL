#include <iostream>
#include <string>

#include "Performance.h"

Performance::Performance(std::string id, Criteria criteria) {
  id_ = id;
  int len = sizeof(criteria) / sizeof(criteria[0]);
  std::pair<std::string, float> perf[len];
  for (int i = 0; i < len; i++) {
    perf[i] = std::make_pair(criteria[i].getId(), 0);
  }
  std::copy(perf, perf + len, perf_);
}

Performance::Performance(std::string id, Criteria criteria, float perf[]) {
  id_ = id;
  int len = sizeof(criteria) / sizeof(criteria[0]);
  std::pair<std::string, float> perf[len];
  for (int i = 0; i < len; i++) {
    perf[i] = std::make_pair(criteria[i].getId(), perf[i]);
  }
  std::copy(perf, perf + len, perf_);
}

Performance::Performance(const Performance &p) {
  id_ = p.getId();
  std::pair<std::string, float> *pperf = &p.getPerf();
  int len = sizeof(pperf) / sizeof(pperf[0]);
  std::pair<std::string, float> perf[len];
  for (int i = 0; i < len; i++) {
    perf[i] = std::make_pair(pperf[i].first, pperf[i].second);
  }
  std::copy(perf, perf + len, perf_);
}