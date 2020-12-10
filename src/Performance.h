#include "AtomicMCDAObject.h"
#include "Criteria.h"
#include <iostream>
#include <string>
#include <vector>

class Performance : public AtomicMCDAObject {
public:
  /**
   * Performance standard constructor
   *
   * @param id Id name of the performance
   * @param criteria Array of criterion to compute performance over
   */
  Performance(std::string id, Criteria &criteria);

  /**
   * Performance standard constructor
   *
   * @param id Id name of the performance
   * @param criteria Array of criterion to compute performance over
   * @param perf[] Array of performance to set
   */
  Performance(std::string id, Criteria &criteria, std::vector<float> &perf);

  /**
   * Performance constructor by copy
   *
   * @param perf Based performance to copy
   */
  Performance(const Performance &perf);

  ~Performance();

  friend std::ostream &operator<<(std::ostream &out, const Performance &p);

  /**
   * getId getter of id parameter
   *
   * @return id
   */
  std::string getId() const;

  /**
   * setId setter of id parameter
   *
   * @param id
   */
  void setId(std::string id);

  /**
   * getPerf getter of weight parameter
   *
   * @return perf
   */
  std::vector<std::pair<std::string, float>> getPerf() const;

private:
  std::string id_;

  // Performance on each criteria represented as a pair of criteria name and
  // perf value
  std::vector<std::pair<std::string, float>> perf_;
};
