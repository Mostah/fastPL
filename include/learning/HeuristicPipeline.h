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
   * @param models list of models to sort
   * @param worstHalf only working on the worst half
   *
   */
  void orderModels(std::vector<MRSortModel> &models, bool worstHalf);

  /** computeAccuracy compute the accuracy of the model given the dataset and
   * store it into the model
   *
   * @param model model to work on
   *
   */
  void computeAccuracy(MRSortModel &model);

private:
  Config &conf;
  AlternativesPerformance &altPerfs;

  WeightUpdater weightUpdater;
  ProfileInitializer profileInitializer;

  // ProfileUpdater profileUpdater;
};

#endif