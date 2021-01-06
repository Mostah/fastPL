#ifndef AlternativePerformance_H
#define AlternativePerformance_H

#include "Criteria.h"
#include "Performance.h"
#include "PerformanceTable.h"
#include <iostream>
#include <map>
#include <ostream>
#include <string.h>
#include <vector>

class AlternativePerformance : public PerformanceTable {
public:
  /**
   * AlternativePerformance standard constructor (PerformanceTable surcharged)
   *
   * @param perf_vect Vector of performance
   * @param alt_assignment Map of alternative assignements to categories
   */
  AlternativePerformance(std::vector<Performance> &perf_vect,
                         std::map<std::string, std::string> alt_assignment);

  /**
   * AlternativePerformance standard constructor without map (PerformanceTable
   * surcharged). AlternativePerformance will be set to unknown categories
   *
   * @param perf_vect Vector of performance
   */
  AlternativePerformance(std::vector<Performance> &perf_vect);

  /**
   * AltAssignemnt constructor without perf values but set of criteria
   * to evaluate performance over (PerformanceTable surcharged)
   *
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   * @param alt_assignment Map of alternative to the assigned category
   */
  AlternativePerformance(int nb_of_perfs, Criteria &crits,
                         std::map<std::string, std::string> alt_assignment);

  /**
   * AltAssignemnt constructor without perf values but set of criteria
   * to evaluate performance over, without map (PerformanceTable surcharged)
   *
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   * @param alt_assignment Map of alternative to the assigned category
   */
  AlternativePerformance(int nb_of_perfs, Criteria &crits);

  /**
   * AlternativePerformance constructor using an existing performance table
   *
   * @param perf_table Performance table to copy
   * @param alt_assignment Map of alternative assignements to categories
   */
  AlternativePerformance(const PerformanceTable &perf_table,
                         std::map<std::string, std::string> alt_assignment);

  /**
   * AlternativePerformance constructor using an existing performance table
   * without assignment
   *
   * @param perf_table Performance table to copy
   */
  AlternativePerformance(const PerformanceTable &perf_table);

  /**
   * AlternativePerformance constructor by copy
   *
   * @param perfs Based performances to copy
   */
  AlternativePerformance(const AlternativePerformance &alt);

  ~AlternativePerformance();

  /**
   * getAlternativePerformanceMap getter of the alternatives assignments
   *
   * @return alt_assignment_
   */
  std::map<std::string, std::string> getAlternativePerformanceMap() const;

  /**
   * getAlternative getter of the assignment of an alternative
   *
   * @param altName name of the alternative of which the assignment
   * (=categoryId) is requested
   * @return assignment (=categoryId) of the alternative
   */
  std::string getAlternativePerformance(std::string altName) const;

private:
  // Hashmap: key = Alternative Name, value = Category Name
  std::map<std::string, std::string> alt_assignment_;
};

#endif