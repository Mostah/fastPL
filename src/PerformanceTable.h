#ifndef PERFORMANCETABLE_H
#define PERFORMANCETABLE_H

#include "Criteria.h"
#include "Performance.h"
#include <iostream>
#include <vector>

class PerformanceTable {
public:
  /**
   * Performances constructor with defined vector of performance. All
   * Performance should have the same criteria to be based on.
   *
   * @param perf_vect Vector of performance
   */
  PerformanceTable(std::vector<Performance> &perf_vect);

  /**
   * Criteria constructor without perf values but set of criteria to evaluate
   * performance over
   *
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   */
  PerformanceTable(std::string prefix, int nb_of_perfs, Criteria crits);

  /**
   * Performances constructor by copy
   *
   * @param perfs Based performances to copy
   */
  PerformanceTable(const PerformanceTable &perfs);

  ~PerformanceTable();

  friend std::ostream &operator<<(std::ostream &out,
                                  const PerformanceTable &perfs);

  /**
   * setPerformanceTable getter of performance table parameter
   *
   * @return performance_table
   */
  std::vector<Performance> getPerformanceTable() const;

  /**
   * Overloading [] operator for PerformanceTable
   *
   * @param name name of the alt or profile we want
   *
   *@return Performance object with associated criterion
   */
  Performance operator[](std::string name) const;

  // TODO
  // Sort (build index?)
  // get middle
  // get best ap, get worst ap
  // display
  // ensure consistency of [][] operator: a performance table should ensure that
  // there is no row initially with different name, at most one row per name, a
  // row cannot have two Perf with the same criteria, and a column is alway set
  // on the same criteria (initially) and throw error at creation if something
  // is wrong.

private:
  std::vector<Performance> pt_;
};

#endif