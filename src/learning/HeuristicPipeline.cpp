#include <algorithm>
#include <vector>

#include "../../include/learning/HeuristicPipeline.h"
#include "../../include/learning/ProfileInitializer.h"
#include "../../include/learning/WeightUpdater.h"
#include "../../include/types/MRSortModel.h"
// #include "../../include/learning/ProfileUpdater.h"

HeuristicPipeline::HeuristicPipeline(Config &config,
                                     AlternativesPerformance &altPerfs)
    : conf(config), altPerfs(altPerfs), weightUpdater(altPerfs, config),
      profileInitializer(config, altPerfs) {
  // ProfileUpdater pu = ProfileUpdater(config, altPerfs);
  // profileUpdater = pu;
}

void HeuristicPipeline::start() {
  conf.logger->info("Starting heuristic pipeline");
  int n_cat = altPerfs.getNumberCats();
  int n_crit = altPerfs.getNumberCrit();

  // First itteration outside the loop: run every algorithm on all models
  // Creation of models and profile initialization
  conf.logger->info("Running 1st itteration on all models");
  for (int k = 0; k < conf.model_batch_size; k++) {
    MRSortModel model = MRSortModel(n_cat, n_crit);
    profileInitializer.initializeProfiles(model);

    // change back to alt mode
    model.profiles.changeMode("alt");
    models.push_back(model);
  }
  // Update weight and lambda
  for (int k = 0; k < conf.model_batch_size; k++) {
    weightUpdater.updateWeightsAndLambda(models[k]);
  }
  // Update profiles
  for (int k = 0; k < conf.model_batch_size; k++) {
    // profileUpdater.updateProfiles(model[k]);
  }
  // compute accuracy, check for convergence and order the models by accuracy
  this->orderModels(false);
  // iterating until convergence or reaching the max iteration, only working on
  // the worst half
  conf.logger->info("Iteration 1 done, best model has an accuracy of: " +
                    std::to_string(models[0].accuracy));
  for (int i = 1; i < conf.max_iterations; i++) {
    // ** profiles initialization **

    // models are sorted in descending order by accuracy
    // re-initialize the worst half of the models
    for (int k = conf.model_batch_size - 1; k > conf.model_batch_size / 2 - 1;
         k--) {
      profileInitializer.initializeProfiles(models[k]);
      models[k].profiles.changeMode("alt");
    }

    // ** Weight and lambda update **
    for (int k = conf.model_batch_size - 1; k > conf.model_batch_size / 2 - 1;
         k--) {
      weightUpdater.updateWeightsAndLambda(models[k]);
    }

    // ** Profiles update **
    for (int k = conf.model_batch_size - 1; k > conf.model_batch_size / 2 - 1;
         k--) {
      // profileUpdater.updateProfiles(models[k]);
    }

    this->orderModels(true);

    // if one model is accurately representing the dataset, stop the learning
    // algorithm
    conf.logger->info("Iteration " + std::to_string(i) +
                      "done, best model has an accuracy of: " +
                      std::to_string(models[0].accuracy));
    if (models[0].accuracy == 1) {
      conf.logger->info(
          "Model with an accuracy of 1 found, stopping algorithm");
      break;
    }
  }
  conf.logger->info("Reaching max iteration, terminating the pipeline");
}

void HeuristicPipeline::customSort() {
  std::vector<int> temp;
  for (int i = 0; i < models.size(); i++) {
    temp.push_back(i);
  }
  auto lambda_models = models;
  std::sort(temp.begin(), temp.end(),
            [lambda_models](const auto &a, const auto &b) {
              return lambda_models[a].accuracy > lambda_models[b].accuracy;
            });
  std::vector<MRSortModel> models_copy;
  for (auto e : models) {
    models_copy.push_back(e);
  }
  models.clear();
  for (auto i : temp) {
    models.push_back(models_copy[i]);
  }
}

void HeuristicPipeline::orderModels(bool worstHalf) {
  int begin = 0;
  int end = models.size();
  if (worstHalf) {
    begin = models.size() / 2;
  }

  for (int k = begin; k < end; k++) {
    this->computeAccuracy(models[k]);
  }
  // for some reason std::sort is not working here...
  this->customSort();
}

void HeuristicPipeline::computeAccuracy(MRSortModel &model) {
  AlternativesPerformance model_assignments =
      model.categoryAssignments(altPerfs);
  std::unordered_map<std::string, Category> truth =
      altPerfs.getAlternativesAssignments();
  std::unordered_map<std::string, Category> assignments =
      model_assignments.getAlternativesAssignments();
  int acc = 0;
  for (std::pair<std::string, Category> e : assignments) {
    if (e.second.getCategoryRank() == truth[e.first].getCategoryRank()) {
      acc++;
    }
  }
  model.accuracy = float(acc) / float(altPerfs.getNumberAlt());
}