#ifndef MRSORTMODEL_H
#define MRSORTMODEL_H

#include <iostream>
#include <string>
#include <vector>

#include "AlternativesPerformance.h"
#include "Categories.h"
#include "Criteria.h"
#include "PerformanceTable.h"
#include "Profiles.h"

class MRSortModel {
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
  MRSortModel(Criteria &criteria, Profiles &profiles, Categories &categories,
              float lambda, std::string id = "model");

  /**
   * MRSortModel generator constructor. This constructor initializes the
   * profiles, lambda and criteria weights at random.
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
   * @return category_assignment AlternativeAssignment object containing the pt
   * given in args and the category assignment.
   */
  AlternativesPerformance categoryAssignment(PerformanceTable &pt);

  Criteria criteria;
  Profiles profiles;
  float lambda;
  Categories &categories;

private:
  std::string id_;
};

#endif