#ifndef MRSORTMODEL_H
#define MRSORTMODEL_H

/**
 * @file MRSortModel.h
 * @brief Model datastructre that will learn from the problem.
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include "AlternativesPerformance.h"
#include "Categories.h"
#include "Criteria.h"
#include "PerformanceTable.h"
#include "Profiles.h"

/** @class MRSortModel MRSortModel.h
 * @brief Datastructure of the model to learn
 *
 * The MRSortModel is the datastructure that represents a complete modelisation
 * of the problem to learn.
 * The state of a model is defined by lambda, a Criteria object and a Profile
 * object. Lambda is a float representing the global threshold of the problem,
 * the Profile object hold the values that defined the profiles delimiting all
 * the categories, and Criteria holds the weights of each Criterion.
 *
 * The model is the "atomic" object that we use in the metaheuristic, it is the
 * datastructure running trough the different pipeline and modified by the
 * learning algorithms.
 *
 * The performance of the model is evaluated by running the ground truth dataset
 * into the model and looking at the category assignment.
 */
class MRSortModel {
public:
  /**
   * MRSortModel standard constructor
   *
   * @param criteria Criteria object
   * @param profiles Profiles containing the profiles
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
   * @param n_cat number of categories
   * @param n_crit number of criteria
   * @param id mrsort model's id
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