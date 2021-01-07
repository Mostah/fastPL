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
   * @param perf_vect Vector of performance
   * @param alt_assignment Map of alternative to the assigned category. Default
   * alternatives assigned to empty categories
   */
  AlternativesPerformance(std::vector<Performance> &perf_vect,
                          std::map<std::string, std::string> alt_assignment =
                              std::map<std::string, std::string>{{"", ""}});

  /**
   * AlternativesPerformance constructor without perf values but set of
   * criteria to evaluate performance over (PerformanceTable surcharged)
   *
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created.
   * Default = "alt"
   * @param alt_assignment Map of alternative to the assigned category. Default
   * alternatives assigned to empty categories
   * */
  AlternativesPerformance(int nb_of_perfs, Criteria &crits,
                          std::string prefix = "alt",
                          std::map<std::string, std::string> alt_assignment =
                              std::map<std::string, std::string>{{"", ""}});

  /**
   * AlternativesPerformance constructor using an existing performance table
   *
   * @param alt_assignment Map of alternative assignements to categories
   * @param perf_table Performance table to copy
   */
  AlternativesPerformance(const PerformanceTable &perf_table,
                          std::map<std::string, std::string> alt_assignment =
                              std::map<std::string, std::string>{{"", ""}});

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