/**
 * @file Profiles.h
 * @brief Profiles data structure.
 *
 */

#ifndef PROFILES_H
#define PROFILES_H

#include "Category.h"
#include "Criteria.h"
#include "Performance.h"
#include "PerformanceTable.h"
#include <iostream>
#include <ostream>
#include <string.h>
#include <vector>

/**
 * @class Profiles Profiles.h
 *
 * The Profiles class hold the datastructure that implement the theoretical
 * definition of a "profile": the values that delimit two categories. A Profiles
 * object is represented as a PerformanceTable that guarantee the right order of
 * the values. In order to have a correct representation of a set of profile (to
 * delimit all the category of our problem), each profile must be dominated by
 * the one above and must demonate the one below.
 * Example:
 *             crit0   crit1
 * category 2
 *              0.6     0.8    --  profile 1
 * category 1
 *              0.4     0.5    --  profile 0
 * category 0
 *
 */
class Profiles : public PerformanceTable {
public:
  /**
   * Profiles standard constructor (PerformanceTable surcharged)
   *
   * @param perf_vect Vector of performance
   *
   */
  Profiles(std::vector<Performance> &perf_vect);

  /**
   * Profiles constructor without perf values but set of
   * criteria to evaluate performance over (PerformanceTable surcharged)
   *
   * @param nb_of_prof Number of profile
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created.
   * Default = "alt"
   * */
  Profiles(int nb_of_prof, Criteria &crits, std::string prefix = "prof");

  /**
   * Profiles constructor by copy
   *
   * @param perfs Based performances to copy
   */
  Profiles(const Profiles &profiles);

  ~Profiles();

  friend std::ostream &operator<<(std::ostream &out, const Profiles &profs);

  /**
   * isProfileOrdered check if the performance table is ordered to be use as a
   * profile type: each row must be ranked such that for each row i we have on
   * each criterion j: v_i-1_j > v_i_j > v_i+1_j.
   * Profiles are therefore ordered in a descending manner
   *
   * @return isOrdered
   */
  bool isProfileOrdered();

  /**
   * generateRandomPerfValues set all the Perf values to random
   *
   * @param seed (optional) Random seed to use in the random generator
   * @param lower_bound (optional) lower bound of the generated Perf values
   * @param upper_bound (optional) upper bound of the generated Perf values
   */
  void generateRandomPerfValues(unsigned long int seed = time(NULL),
                                int lower_bound = 0, int upper_bound = 1);
};

#endif