#ifndef HEURISTICPIPELINE_H
#define HEURISTICPIPELINE_H

#include <vector>

#include "../app.h"
#include "ProfileInitializer.h"
#include "WeightUpdater.h"
// #include "ProfileUpdater"

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
  void start();

  /** orderModels Order models by their accuracy.
   *
   * @param worstHalf only working on the worst half
   *
   */
  void orderModels(bool worstHalf);

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
  // ProfileUpdater profileUpdater;
};

#endif