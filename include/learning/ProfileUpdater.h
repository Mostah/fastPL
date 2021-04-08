#ifndef PROFILEUPDATER_H
#define PROFILEUPDATER_H

/** @file ProfileUpdater.h
 *  @brief Third step of the heuristic, updates the profiles given fixed weight
 * and lambda.
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include "../app.h"
#include "../types/MRSortModel.h"

/** @class ProfileUpdater ProfileUpdater.h
 *  @brief Third step of the heuristic, updates the profiles given fixed weight
 * and lambda.
 *
 * Profile Updater algorithm is responsible for updating the profiles given the
 * fixed weight and lambda. It will evaluate each move of each profile on each
 * criterion to see what moves have the best impact on the alternatives
 * assignments.
 *
 *
 */

class ProfileUpdater {
public:
  /**
   * ProfileUpdater standard constructor
   *
   * @param conf conf setup for logging
   * @param altPerf_data alternative perfomances dataset
   * @param epsilon
   */
  ProfileUpdater(Config &conf, AlternativesPerformance &altPerf_data,
                 float epsilon = 0.00001);

  /**
   * ProfileUpdater copy constructor
   *
   * @param profUp ProfileUpdater to copy
   */
  ProfileUpdater(const ProfileUpdater &profUp);

  ~ProfileUpdater();

  /**
   * computeAboveDesirability computes the desirability of a move for the
   * profile b (move up)
   *
   * @param model current model
   * @param critId crit on which we want to evaluate the move
   * @param b profile we want to move
   * @param b_above profile above
   * @param cat category delimited by the profile (below)
   * @param cat_above above category delimited by the profile
   * @param ct_prof concordance table of the profile
   * @param altPerf_model alternativePerformance calculated with current model
   *
   * @return the map of potential new perf for the profile and their
   * desirability
   */
  std::unordered_map<float, float>
  computeAboveDesirability(MRSortModel &model, std::string critId, Perf &b,
                           Perf &b_above, Category &cat, Category &cat_above,
                           std::unordered_map<std::string, float> &ct_prof,
                           AlternativesPerformance &altPerf_model);

  /**
   * computeBelowDesirability computes the desirability of a move for the
   * profile b (move down)
   *
   * @param model current model
   * @param critId crit on which we want to evaluate the move
   * @param b profile we want to move
   * @param b_below profile below
   * @param cat category delimited by the profile (below)
   * @param cat_above above category delimited by the profile
   * @param ct_prof concordance table of the profile
   * @param altPerf_model alternativePerformance calculated with current model
   *
   * @return the map of potential new perf for the profile and their
   * desirability
   */
  std::unordered_map<float, float>
  computeBelowDesirability(MRSortModel &model, std::string critId, Perf &b,
                           Perf &b_below, Category &cat, Category &cat_above,
                           std::unordered_map<std::string, float> &ct_prof,
                           AlternativesPerformance &altPerf_model);

  /**
   * chooseMaxDesirability chooses the move of the profiles that maximizes the
   * desirability
   *
   * @param desirability map of desirability
   * @param b profile to move
   *
   * @returns profile value and associate desirability (max)
   */
  std::pair<float, float>
  chooseMaxDesirability(std::unordered_map<float, float> &desirability,
                        Perf &b);

  /**
   * updateTables updates model tables with new profile value
   * - concordance table
   * - alternative assignement
   * - profiles
   *
   * @param model
   * @param critId criterion on which the profile moves
   * @param b_old old profile perf
   * @param b_new new profile perf
   * @param ct concordance table to update
   * @param altPerf_model alternativePerformance calculated with current model
   *
   */
  void updateTables(
      MRSortModel &model, std::string critId, Perf &b_old, Perf &b_new,
      std::unordered_map<std::string, std::unordered_map<std::string, float>>
          &ct,
      AlternativesPerformance &altPerf_model);

  /**
   * optimizeProfile Optimizes one profile using the profileUpdater methods.
   *
   * @param prof profile to optimize
   * @param cat_below category delimited by the profile (below)
   * @param cat_above category delimited by the profile (above)
   * @param model current model
   * @param ct concordance table
   * @param altPerf_model altPerf_model
   *
   */
  void optimizeProfile(
      std::vector<Perf> &prof, Category &cat_below, Category &cat_above,
      MRSortModel &model,
      std::unordered_map<std::string, std::unordered_map<std::string, float>>
          &ct,
      AlternativesPerformance &altPerf_model);

  /**
   * optimize Optimizes all the profiles using the profileUpdater methods.
   *
   * @param model current model
   * @param ct concordance table
   * @param altPerf_model altPerf_model
   *
   */
  void optimize(MRSortModel &model,
                std::unordered_map<std::string,
                                   std::unordered_map<std::string, float>> &ct,
                AlternativesPerformance &altPerf_model);

  /**
   * updateProfiles Updates the profiles of the model using the metaheuristic
   *
   * @param model current model
   *
   */
  void updateProfiles(MRSortModel &model);

private:
  float epsilon_;
  AlternativesPerformance &altPerf_data;
  Config &conf;
};

#endif