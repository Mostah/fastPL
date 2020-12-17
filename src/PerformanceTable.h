#ifndef PERFORMANCETABLE_H
#define PERFORMANCETABLE_H

#include "Criteria.h"
#include "Performance.h"
#include <ctime>
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
   * @param prefix Prefix to use for the name of each Performance created
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

  /**
   * generateRandomPerfValues set all the Perf values to random
   *
   * @param seed (optional) Random seed to use in the random generator
   */
  void generateRandomPerfValues(unsigned long int seed = time(NULL));

  friend std::ostream &operator<<(std::ostream &out,
                                  const PerformanceTable &perfs);

  /**
   * getPerformanceTable getter of performance table parameter
   *
   * @return performance_table
   */
  std::vector<std::vector<Perf>> getPerformanceTable() const;

  /**
   * getMode getter of the mode of the performance table
   *
   * @return mode
   */
  std::string getMode() const;

  /**
   * isSorted getter of the sorted variable
   *
   * @return sorted
   */
  bool isSorted() const;

  /**
   * Overloading [] operator for PerformanceTable. If the current mode is crit,
   * will search for the row with the crit name given. If current mode is alt,
   * will search for the row with the alternative (or profile) name given.
   *
   * @param name name of the row we want to search
   *
   *@return Performance object with associated criterion
   */
  std::vector<Perf> operator[](std::string name) const;

  /**
   * getPerf getter of a specific Perf in the performance table
   *
   * @param name name of the alt or profile we want
   * @param crit name of the crit we want
   */
  Perf getPerf(std::string name, std::string crit) const;

  // TODO
  // get middle
  // get best ap, get worst ap

  /**
   * sort Sort the performance table given the selected mode: alt or crit.
   * Sorted by alt will assign the first dimension to alt and sort the second by
   * crit. Sorting by crit will assign the first dimension to crit and sort the
   * second dimension by alt.
   *
   * @param mode selected pt mode (alt or crit)
   */
  void sort(std::string mode = "crit");

  /**
   * changeMode Change the assigned mode to the performance table given the
   * selected one: alt or crit. alt mode will assign the first dimension to alt
   * and second to crit. crit mode will assign the first dimension to crit and
   * the second dimension to alt.
   *
   * @param mode selected pt mode (alt or crit)
   */
  void changeMode(std::string mode);

  /**
   * getAltBetween return all the alternatives / profiles that have a
   * performance (value) between inf and sup on criterion crit. The performance
   * table must have been sorted before calling this function.
   *
   * @param critId crit Id to look for
   * @param inf inferior boudary
   * @param sup superior boundary
   *
   * @return sub_vect
   */
  std::vector<Perf> getAltBetween(std::string critId, float inf, float sup);

  /**
   * getBestPerfByRow return a perf vector with the best
   * performance of the alternatives on each criteria
   *
   *
   * @return best_pv
   */
  std::vector<Perf> getBestPerfByCrit(Criteria crits);

  /**
   * getWorstPerfByRow return a perf vector with the worst
   * performance of the alternatives on each criteria
   *
   * @return worst_pv
   */
  std::vector<Perf> getWorstPerfByCrit(Criteria crits);

private:
  std::vector<std::vector<Perf>> pt_;

  // indicates what is represented by rows: (alt or profiles) or criterias
  std::string mode_ = "alt"; // takes its value in {"crit", "alt"}
  bool sorted_ = false;
};

#endif