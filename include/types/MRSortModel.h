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
   * @param ap alternative performance object
   * @param id optional name of the model
   * @param mode give the mode of the profile performance table
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

private:
  std::string id_;
};

#endif