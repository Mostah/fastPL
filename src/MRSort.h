#ifndef MRSORT_H
#define MRSORT_H

#include <iostream>
#include <string>
#include <vector>

#include "AtomicMCDAObject.h"
#include "Criteria.h"
#include "PerformanceTable.h"

class MRSort : public AtomicMCDAObject {
public:
  /**
   * MRSort standard constructor
   *
   * @param criteria Criteria object
   * @param ap alternative performance object
   * @param lambda threshold
   * @param id optional name of the model
   */
  MRSort(Criteria &criteria, PerformanceTable &ap, float lambda,
         std::string id = "model");

  /**
   * MRSort constructor by copy
   *
   * @param mrsort MRSort object to copy
   */
  MRSort(const MRSort &mrsort);
  ~MRSort();

  friend std::ostream &operator<<(std::ostream &out, const MRSort &mrsort);

  /**
   * getCriteria getter of criteria parameter
   *
   * @return criteria
   */
  Criteria getCriteria() const;

  /**
   * getAp getter of alternative performance parameter
   *
   * @return ap
   */
  PerformanceTable getAp() const;

  /**
   * getLambda getter of lambda parameter
   *
   * @return lambda
   */
  float getLambda() const;

  /**
   * getId getter of id parameter
   *
   * @return id
   */
  std::string getId() const;

private:
  Criteria &criteria_;
  // Will be changed for Alternative Performance once defined
  PerformanceTable &ap_;
  float lambda_;
  std::string id_;
};

#endif