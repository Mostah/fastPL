#ifndef MRSORTMODEL_H
#define MRSORTMODEL_H

#include <iostream>
#include <string>
#include <vector>

#include "AtomicMCDAObject.h"
#include "Criteria.h"
#include "PerformanceTable.h"

class MRSortModel : public AtomicMCDAObject {
public:
  /**
   * MRSortModel standard constructor
   *
   * @param criteria Criteria object
   * @param lambda threshold
   * @param id optional name of the model
   */
  MRSortModel(Criteria &criteria, PerformanceTable &profiles, float lambda,
              std::string id = "model");

  /**
   * MRSortModel generator constructor. This
   * constructor initializes the profiles, lambda and criteria weights at
   * random.
   *
   * @param ap alternative performance object
   * @param id optional name of the model
   */
  MRSortModel(int n_cat, int n_crit, std::string id = "model");

  /**
   * MRSortModel constructor by copy
   *
   * @param mrsort MRSortModel object to copy
   */
  MRSortModel(const MRSortModel &mrsort);
  ~MRSortModel();

  friend std::ostream &operator<<(std::ostream &out, const MRSortModel &mrsort);

  /**
   * getId getter of id parameter
   *
   * @return id
   */
  std::string getId() const;

  Criteria criteria;
  // Will be changed for Alternative Performance once defined
  PerformanceTable profiles;
  float lambda;

private:
  std::string id_;
};

#endif