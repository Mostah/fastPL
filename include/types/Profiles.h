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

class Profiles : public PerformanceTable {
public:
  /**
   * Profiles standard constructor (PerformanceTable surcharged)
   *
   * @param perf_vect Vector of performance that models category limits
   * @param mode give the mode of the profile performance table
   *
   */
  Profiles(std::vector<Performance> &perf_vect, std::string mode);

  /**
   * Profiles constructor without perf values but set of
   * criteria to evaluate performance over (PerformanceTable surcharged)
   *
   * @param nb_of_prof Number of profile
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created.
   * @param mode give the mode of the profile performance table
   * Default = "alt"
   * */
  Profiles(int nb_of_prof, Criteria &crits, std::string mode,
           std::string prefix = "prof");

  /**
   * Method that changes Profile mode and gets the order of the profile PT
   */
  Profiles changeMode();

  /**
   * Profiles constructor by copy
   *
   * @param perfs Based performances to copy
   */
  Profiles(const Profiles &profiles);

  ~Profiles(){};

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

  /**
   * Function that will display the profile performance table in a elegant way
   */
  void display();
};

#endif