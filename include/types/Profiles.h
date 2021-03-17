#ifndef PROFILES_H
#define PROFILES_H

#include "Category.h"
#include "Criteria.h"
#include "Perf.h"
#include "PerformanceTable.h"
#include <iostream>
#include <ostream>
#include <string.h>
#include <vector>

class Profiles : public PerformanceTable {
public:
  /**
   * Profiles standard constructor (PerformanceTable surcharged). A Profile type
   * is always represented in the mode_ = "crit" of a PerformanceTable. There is
   * no such thing as a Profile in "alt" mode.
   *
   * @param perf_vect Vector of performance that models category limits or a
   * PerformanceTable Profiles in "alt"
   * @param mode refers the mode of the Profiles
   *
   */
  Profiles(std::vector<std::vector<Perf>> &perf_vect, std::string mode);

  /**
   * Profiles constructor without perf values but set of
   * criteria to evaluate performance over (PerformanceTable surcharged)
   *
   * @param nb_of_prof Number of profile
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created.
   * @param mode refers the mode of the Profiles
   *
   * */
  Profiles(int nb_of_prof, Criteria &crits, std::string mode,
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
   * isProfileOrdered check if the performance table is ordered to be use as a
   * profile type: each row must be ranked such that for each row i is sorted in
   * ascending order.
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
   * getBelowAndAboveProfile gets the profiles below and above the given
   * profile. If given profile is the first or last, it will return itself
   *
   * @param profId id of the profile to get the above and below profiles
   *
   * @return profiles performances (below and above)
   */
  std::pair<std::vector<Perf>, std::vector<Perf>>
  getBelowAndAboveProfile(std::string profName);

  /**
   * setPerf set a Perf given the alt name, crit and value.
   * Unefficient since using [] operator...
   *
   * @param name name of the alt or profile we want to update
   * @param crit name of the crit we want to update
   * @param value new perf value
   */
  void setPerf(std::string name, std::string crit, float value);
};

#endif