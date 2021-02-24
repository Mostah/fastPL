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
   * @param epsilon
   */
  ProfileUpdater(AlternativesPerformance &altPerf_data,
                 float epsilon = 0.00001);

  /**
   * ProfileUpdater copy constructor
   *
   * @param profUp ProfileUpdater to copy
   */
  ProfileUpdater(const ProfileUpdater &profUp);

  ~ProfileUpdater();

  std::unordered_map<float, float>
  computeAboveDesirability(MRSortModel &model, std::string critId, Perf &b,
                           Perf &b_above, Category &cat, Category &cat_above,
                           std::unordered_map<std::string, float> &ct_prof);

  std::unordered_map<float, float>
  computeBelowDesirability(MRSortModel &model, std::string critId, Perf &b,
                           Perf &b_below, Category &cat, Category &cat_above,
                           std::unordered_map<std::string, float> &ct_prof);

private:
  float epsilon_;
  AlternativesPerformance &altPerf_data;
};

#endif