#ifndef PERFORMANCETABLE_H
#define PERFORMANCETABLE_H

#include "Criteria.h"
#include "Performance.h"
#include <iostream>
#include <vector>

class PerformanceTable {
public:
  /**
   * Performances constructor with defined vector of performance
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

  // TODO
  // Sort (build index?)
  // get middle
  // get best ap, get worst ap
  // display
  // changer Performance:
  // Perf = {name: .. crit: .. , val: .. }
  // Performance = Vector<Perf>
  // PerformanceTable = Vector<Performance>

private:
  std::vector<Performance> performance_table_;
};

#endif