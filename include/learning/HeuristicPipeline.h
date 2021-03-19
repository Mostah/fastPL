#ifndef HEURISTICPIPELINE_H
#define HEURISTICPIPELINE_H

/**
 * @file HeuristicPipeline.h
 * @brief Global heuristic to learn a model.
 *
 */

#include <vector>

#include "../app.h"
#include "ProfileInitializer.h"
#include "ProfileUpdater.h"
#include "WeightUpdater.h"

/** @class HeuristicPipeline HeuristicPipeline.h
 *  @brief Global heuristic to learn a model.
 *
 * The heuristic pipeline is responsible of initializing the sub algorithms: the
 * WeightUpdater, the ProfileInitializer and the ProfileUpdater, is responsible
 * for initilizing the population of n models and run the learning metaheuristic
 * to converge into a learned model.
 *
 * A complete description of the heuristic can be found in @subpage
 * learning_algorithms.
 */

class HeuristicPipeline {

public:
  /** HeuristicPipeline Base constructor, stores the given config.
   *
   * @param config app config to start and run the application.
   */
  HeuristicPipeline(Config &config, AlternativesPerformance &altPerfs);

  /** Start run the heuristic pipeline
   *
   */
  MRSortModel start();

  /** orderModels Order models by their accuracy.
   *
   */
  void orderModels();

  /**
   * customSort sort the model given their accuracy
   *
   * @param model_vect vector of model to be sorted
   */
  void customSort();

  /** computeAccuracy compute the accuracy of the model given the dataset and
   * store it into the model
   *
   * @param model model to work on
   *
   */
  void computeAccuracy(MRSortModel &model);

  std::vector<MRSortModel> models;

private:
  Config &conf;
  AlternativesPerformance &altPerfs;

  WeightUpdater weightUpdater;
  ProfileInitializer profileInitializer;
  ProfileUpdater profileUpdater;
};

#endif