#ifndef PERFORMANCETABLE_H
#define PERFORMANCETABLE_H

/**
 * @file PerformanceTable.h
 * @brief PerformanceTable data structure.
 *
 */

#include "Criteria.h"
#include "Perf.h"
#include <ctime>
#include <iostream>
#include <vector>

/**
 * @class PerformanceTable PerformanceTable.h
 * @brief PerformanceTable data structure.
 *
 * PerformanceTable is a table of Perf objects. It is a Mathematical
 * datastructure that can be interpreted as a Matrix if a few conditions are
 * met, or a vector of vector if the column order cannot be guaranteed.
 *
 * The PerformanceTable has to mode of representation: crit and alt.
 *   -  In crit mode, the table is index by crit in the first dimension (in one
 * row we have all perf of a certain crit), the second dimension beeing alt.
 *   -  In alt mode, the table is index by alt in the first dimension (in one
 * row we have all perf of a certain alt), the second dimension beeing crit.
 *
 * The Perf table can be interpreted as a Matrix if the second dimension has the
 * same order for all row.
 *
 * This datastructure was designed to be itterated over easily, thus the access
 * time of a certain element (like in a hashmap) is in O(n_crit * n_alt) which
 * is absolutely inefficient and should be avoided.
 */
class PerformanceTable {
public:
  /**
   * PerformanceTable constructor with defined vector of performance that
   * represents the PerformanceTable in a certain mode. All Vector of Perf
   * objects should have the same criteria. Need to give the mode of the
   * Performance table that is either "alt" or "crit".
   *
   * @param perf_vect Vector of performance
   */
  PerformanceTable(std::vector<std::vector<Perf>> &perf_vect,
                   std::string mode = "alt");

  /**
   * PerformanceTable constructor without perf values but set of criteria to
   * evaluate performance over. perf values will be set to 0.
   *
   * @param prefix Prefix to use for the name of each Performance created
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   */
  PerformanceTable(int nb_of_perfs, Criteria &crits,
                   std::string prefix = "alt");

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
   * @param lower_bound (optional) lower bound of the generated Perf values
   * @param upper_bound (optional) upper bound of the generated Perf values
   */
  void generateRandomPerfValues(unsigned long int seed = time(NULL),
                                int lower_bound = 0, int upper_bound = 1);

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
   * Overloading [] operator for PerformanceTable. If the current mode is
   *crit, will search for the row with the crit name given. If current mode
   *is alt, will search for the row with the alternative (or profile) name
   *given.
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
   * table must have been sorted and in mode crit before calling this function.
   *
   * @param critId crit Id to look for
   * @param inf inferior boudary
   * @param sup superior boundary
   *
   * @return sub_vect
   */
  std::vector<Perf> getAltBetweenSorted(std::string critId, float inf,
                                        float sup);

  /**
   * getAltBetween return all the alternatives / profiles that have a
   * performance (value) between inf and sup on criterion crit. The performance
   * table does not need to be sorted before calling this function.
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
   * @param crits Criteria to search in the performance table
   *
   * @return best_pv
   */
  std::vector<Perf> getBestPerfByCrit(Criteria &crits);

  /**
   * getWorstPerfByRow return a perf vector with the worst
   * performance of the alternatives on each criteria
   *
   * @param crits Criteria to search in the performance table
   *
   * @return worst_pv
   */
  std::vector<Perf> getWorstPerfByCrit(Criteria &crits);

  /**
   * findAlt return true if the alternative is in the performance table
   *
   * @param altName Alternative to lookup
   *
   * @return true if found, false if nots
   */
  bool isAltInTable(std::string altName);

  /**

   * getNumberCrit return the amount of crits in the table
   *
   * @return n_crit
   */
  int getNumberCrit();

  /**
   * getNumberAlt return the amount of alts in the table
   *
   * @return n_alts
   */
  int getNumberAlt();

  /**
   * Display PerformanceTable in a nice manner. Please be advised that this
   * method might be counter intuitive since elements do not necessarly have a
   * correct order depending on the mode
   *
   */
  void display();

  /**
   * Overload of == operator for PerformanceTable object. It compares the name,
   * criteria and values of the PerformanceTable.
   *
   * @param pt PerformanceTable object to compare it with
   */
  bool operator==(const PerformanceTable &pt) const;

protected:
  std::vector<std::vector<Perf>> pt_;

  // indicates what is represented by rows: (alt or profiles) or criterias
  std::string mode_ = "alt"; // takes its value in {"crit", "alt"}
  bool sorted_ = false;
};

#endif