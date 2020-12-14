#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "Criteria.h"
#include "Perf.h"
#include <iostream>
#include <string>
#include <vector>

class Performance {
public:
  /**
   * Performance standard constructor
   *
   * @param id Id name of the performance (ex: name of alternative or profile)
   * @param criteria Array of criterion to compute performance over
   */
  Performance(std::string id, Criteria &criteria);

  /**
   * Performance standard constructor
   *
   * @param id Id name of the performance (ex: name of alternative or profile)
   * @param criteria Array of criterion to compute performance over
   * @param performance Array of performance (float) to set
   */
  Performance(std::string id, Criteria &criteria, std::vector<float> &p);

  /**
   * Performance standard constructor
   *
   * @param performance[] Array of Perf to set
   */
  Performance(std::vector<Perf> &p);

  /**
   * Performance constructor by copy
   *
   * @param perf Based performance to copy
   */
  Performance(const Performance &p);

  ~Performance();

  friend std::ostream &operator<<(std::ostream &out, const Performance &p);

  /**
   * getPerf getter of weight parameter
   *
   * @return perf
   */
  std::vector<Perf> getPerf() const;

private:
  // Performance on each criteria represented as a pair of criteria name and
  // perf value
  std::vector<Perf> performance_;
};

#endif