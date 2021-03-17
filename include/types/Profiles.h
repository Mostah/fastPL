#ifndef PROFILES_H
#define PROFILES_H

/**
 * @file Profiles.h
 * @brief Profiles data structure.
 *
 */

#include "Category.h"
#include "Criteria.h"
#include "Perf.h"
#include "PerformanceTable.h"
#include <iostream>
#include <ostream>
#include <string.h>
#include <vector>

/**
 * @class Profiles Profiles.h
 * @brief Profiles data structure.
 *
 * The Profiles class hold the datastructure that implement the theoretical
 * definition of a "profile": the values that delimit two categories. A Profiles
 * object is represented as a PerformanceTable that guarantee the right order of
 * the values. In order to have a correct representation of a set of profile (to
 * delimit all the category of our problem), each profile must be dominated by
 * the one above and must demonate the one below.
 *
 * Example:
 *
 * |           |crit0|  crit1|                 |
 * |-----------|-----|-------|-----------------|
 * |category 2 |     |       |                 |
 * |           | 0.6 |   0.8 |  --  profile 1  |
 * |category 1 |     |       |                 |
 * |           | 0.4 |   0.5 |  --  profile 0  |
 * |category 0 |     |       |                 |
 *
 */
class Profiles : public PerformanceTable {
public:
  /**
   * Profiles standard constructor (PerformanceTable surcharged).
   * Default mode is "crit"
   *
   * @param perf_vect Vector of performance that models category limits or a
   * PerformanceTable Profiles in "alt"
   * @param mode refers the mode of the Profiles
   *
   */
  Profiles(std::vector<std::vector<Perf>> &perf_vect,
           std::string mode = "crit");

  /**
   * Profiles constructor without perf values but set of
   * criteria to evaluate performance over (PerformanceTable surcharged)
   * Default mode is "crit".
   *
   * @param nb_of_prof Number of profile
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created.
   * @param mode refers the mode of the Profiles
   *
   * */
  Profiles(int nb_of_prof, Criteria &crits, std::string mode = "crit",
           std::string prefix = "prof");

  /**
   * Profiles constructor by copy
   *
   * @param perfs Based performances to copy
   */
  Profiles(const Profiles &profiles);

  ~Profiles(){};

  /**
   * Overloading << operator for Profile class
   *
   * @param out ostream
   * @param profs Profile object
   *
   */
  friend std::ostream &operator<<(std::ostream &out, const Profiles &profs);

  /**
   * isProfileOrdered checks if the profile performance table is ordered.
   * - "alt" mode : each row of the performance table denotes a profile.
   * The first row is profile b0 refering to category of rank 0.
   * Hence, each criterion must be ordered in descending order.
   * ie : for all i,j : b_i+1_j > b_i_j
   *
   * - "crit" mode : each row of the performance table denotes a criteria
   * modeled by its category limits.
   * Hence, each criterion is independant to one another but for one criterion
   * each category limit (ie column) must be ordered in descending order.
   * The first column is cat0 refering to category of rank 0.
   * ie : for all criterion (row) and i : cat_limit_i+1 > cat_limit_i
   *
   * @return isOrdered
   */
  bool isProfileOrdered();

  /**
   * generateRandomPerfValues set all the Perf values to random according to
   * profile mode
   *
   * @param seed (optional) Random seed to use in the random generator
   * @param lower_bound (optional) lower bound of the generated Perf values
   * @param upper_bound (optional) upper bound of the generated Perf values
   */
  void generateRandomPerfValues(unsigned long int seed = time(NULL),
                                int lower_bound = 0, int upper_bound = 1);
};

#endif