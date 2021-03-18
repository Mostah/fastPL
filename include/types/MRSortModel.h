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
              float lambda, std::string id = "model", float score = 0);

  /**
   * MRSortModel generator constructor. This constructor initializes the
   * profiles, lambda and criteria weights at random.
   *
   * @param ap alternative performance object
   * @param id optional name of the model
   * @param mode give the mode of the profile performance table
   */
  MRSortModel(int n_cat, int n_crit, std::string id = "model", float score = 0);

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
   * getScore getter of score parameter
   *
   * @return score
   */
  float getScore() const;

  /**
   * setScore setter of score parameter
   *
   * @param score new score
   *
   */
  void setScore(float score);

  /**
   * categoryAssignment assign the category given the alternative
   * and the current state of the model.
   *
   * @param alt PerfVect of the alternative
   * @param profiles_pt PerfTable of the profiles
   *
   * @return category_assignment Category object associated to the alternative
   */
  Category categoryAssignment(std::vector<Perf> &alt,
                              std::vector<std::vector<Perf>> &profiles_pt);

  /**
   * categoryAssignments assign the categories given the performance table
   * and the current state of the model.
   *
   * @param pt PerformanceTable
   *
   * @return category_assignment AlternativeAssignment object containing the pt
   * given in args and the category assignment.
   */
  AlternativesPerformance categoryAssignments(PerformanceTable &pt);

  /**
   * computeConcordance computes the concordance value between a profile and an
   * alternative
   *
   * @param prof profile
   * @param alt alternative
   *
   * @return concordance value
   */
  float computeConcordance(std::vector<Perf> &prof, std::vector<Perf> &alt);

  /**
   * computeConcordanceTable computes the concordance table of a performance
   * table
   *
   * @param pt PerformanceTable
   *
   * @return concordance table giving the concordance value for each profile,
   * alternative tupple, displayed in a map { prof_id1 : {alt_id1 : c1,
   alt_id2:
   * c2, ...}, ...}
   */
  std::unordered_map<std::string, std::unordered_map<std::string, float>>
  computeConcordanceTable(PerformanceTable &pt);

  Criteria criteria;
  Profiles profiles;
  float lambda;
  Categories &categories;

  float accuracy;

private:
  std::string id_;
  float score_;
};

#endif