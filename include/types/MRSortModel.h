#ifndef MRSORTMODEL_H
#define MRSORTMODEL_H

#include <iostream>
#include <string>
#include <vector>

#include "AlternativesPerformance.h"
#include "AtomicMCDAObject.h"
#include "Categories.h"
#include "Criteria.h"
#include "PerformanceTable.h"

class MRSortModel : public AtomicMCDAObject {
public:
  /**
   * MRSortModel standard constructor
   *
   * @param criteria Criteria object
   * @param profiles PerformanceTable containing the profiles
   * @param categories Categories object used for the profiles
   * @param lambda threshold
   * @param id optional name of the model
   */
  MRSortModel(Criteria &criteria, PerformanceTable &profiles,
              Categories &categories, float lambda, std::string id = "model");

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

  /**
   * categoryAssignment assign the categories given the performance table
   * and the current state of the model.
   *
   * @param pt PerformanceTable
   *
   * @return category_assignment pair vector with first value is the alternative
   * id and second value the category assigned.
   */
  AlternativesPerformance categoryAssignment(PerformanceTable &pt);

  Criteria criteria;
  PerformanceTable profiles;
  float lambda;
  Categories categories;

private:
  std::string id_;
};

#endif