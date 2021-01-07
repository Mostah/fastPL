#ifndef AlternativesPerformance_H
#define AlternativesPerformance_H

#include "Criteria.h"
#include "Performance.h"
#include "PerformanceTable.h"
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <string.h>
#include <vector>

class AlternativesPerformance : public PerformanceTable {
public:
  /**
   * AlternativesPerformance standard constructor (PerformanceTable surcharged)
   *
   * @param alt_assignment Map of alternative assignements to categories
   * @param perf_vect Vector of performance
   */
  AlternativesPerformance(std::map<std::string, std::string> alt_assignment,
                          std::vector<Performance> &perf_vect);

  /**
   * AlternativesPerformance standard constructor without map (PerformanceTable
   * surcharged). AlternativesPerformance will be set to unknown categories
   *
   * @param perf_vect Vector of performance
   */
  AlternativesPerformance(std::vector<Performance> &perf_vect);

  /**
   * AltAssignemnt constructor without perf values but set of criteria
   * to evaluate performance over (PerformanceTable surcharged)
   *
   * @param alt_assignment Map of alternative to the assigned category
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance createds
   */
  AlternativesPerformance(std::map<std::string, std::string> alt_assignment,
                          int nb_of_perfs, Criteria &crits,
                          std::string prefix = "alt");

  /**
   * AltAssignemnt constructor without perf values but set of criteria
   * to evaluate performance over, without map (PerformanceTable surcharged)
   *
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created
   */
  AlternativesPerformance(int nb_of_perfs, Criteria &crits,
                          std::string prefix = "alt");

  /**
   * AlternativesPerformance constructor using an existing performance table
   *
   * @param alt_assignment Map of alternative assignements to categories
   * @param perf_table Performance table to copy
   */
  AlternativesPerformance(std::map<std::string, std::string> alt_assignment,
                          const PerformanceTable &perf_table);

  /**
   * AlternativesPerformance constructor using an existing performance table
   * without assignment
   *
   * @param perf_table Performance table to copy
   */
  AlternativesPerformance(const PerformanceTable &perf_table);

  /**
   * AlternativesPerformance constructor by copy
   *
   * @param perfs Based performances to copy
   */
  AlternativesPerformance(const AlternativesPerformance &alt);

  ~AlternativesPerformance();

  friend std::ostream &operator<<(std::ostream &out,
                                  const AlternativesPerformance &alt);

  /**
   * getAlternativesPerformanceMap getter of the alternatives assignments
   *
   * @return alt_assignment_
   */
  std::map<std::string, std::string> getAlternativesAssignments() const;

  /**
   * getAlternative getter of the assignment of one specified alternative
   *
   * @param altName name of the alternative of which the assignment
   * (=categoryId) is requested
   * @return assignment (=categoryId) of the alternative
   */
  std::string getAlternativeAssignment(std::string altName) const;

private:
  // Hashmap: key = Alternative Name, value = Category Name
  std::map<std::string, std::string> alt_assignment_;
};

#endif