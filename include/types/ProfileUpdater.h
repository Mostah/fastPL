#ifndef PROFILEUPDATER_H
#define PROFILEUPDATER_H

#include <iostream>
#include <string>
#include <vector>

#include "MRSortModel.h"

class ProfileUpdater {
public:
  /**
   * ProfileUpdater standard constructor
   *
   * @param delta
   */
  ProfileUpdater(float delta = 0.00001);

  /**
   * ProfileUpdater copy constructor
   *
   * @param profUp ProfileUpdater to copy
   */
  ProfileUpdater(const ProfileUpdater &profUp);

  ~ProfileUpdater();

  std::unordered_map<std::string, float> computeAboveDesirability(
      MRSortModel model, AlternativeAssignment altPerf_data, std::string critId,
      Perf perf_prof, Perf perf_prof_above, Category cat_below,
      Category cat_above, std::unordered_map<std::string, float> ct_prof);

private:
  float delta_;
};

#endif