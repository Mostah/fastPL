#include <algorithm>
#include <vector>

#include "../../include/learning/HeuristicPipeline.h"
#include "../../include/learning/ProfileInitializer.h"
#include "../../include/learning/ProfileUpdater.h"
#include "../../include/learning/WeightUpdater.h"
#include "../../include/types/MRSortModel.h"

HeuristicPipeline::HeuristicPipeline(Config &config,
                                     AlternativesPerformance &altPerfs)
    : conf(config), altPerfs(altPerfs), weightUpdater(altPerfs, config),
      profileInitializer(config, altPerfs), profileUpdater(config, altPerfs) {}

MRSortModel HeuristicPipeline::start() {
  conf.logger->info("Starting heuristic pipeline");
  int n_cat = altPerfs.getNumberCats();
  int n_crit = altPerfs.getNumberCrit();

  // First itteration outside the loop: run every algorithm on all models
  // Creation of models and profile initialization
  conf.logger->info("Running 1st itteration on all models");
  using clock = std::chrono::system_clock;
  using sec = std::chrono::duration<double>;
  // for milliseconds, use using ms = std::chrono::duration<double, std::milli>;

  const auto before_init = clock::now();
  for (int k = 0; k < conf.model_batch_size; k++) {
    MRSortModel model = MRSortModel(n_cat, n_crit);

    profileInitializer.initializeProfiles(model);
    // change back to alt mode
    model.profiles.changeMode("alt");
    this->computeAccuracy(model);
    std::cout << "accuracy after init: " << model.getScore() << std::endl;
    models.push_back(model);
  }
  const sec init_duration = clock::now() - before_init;
  // Update weight and lambda
  const auto before_weight = clock::now();
  for (int k = 0; k < conf.model_batch_size; k++) {
    weightUpdater.updateWeightsAndLambda(models[k]);
    this->computeAccuracy(models[k]);
    std::cout << "accuracy after weight update: " << models[k].getScore()
              << std::endl;
  }
  const sec weight_duration = clock::now() - before_weight;
  const auto before_profile = clock::now();
  // Update profiles
  for (int k = 0; k < conf.model_batch_size; k++) {
    profileUpdater.updateProfiles(models[k]);
    this->computeAccuracy(models[k]);
    std::cout << "accuracy after profile update: " << models[k].getScore()
              << std::endl;
  }
  const sec profile_duration = clock::now() - before_profile;
  // compute getScore(), check for convergence and order the models by
  // getScore()
  std::cout << "Intt took " << init_duration.count() << "s" << std::endl;
  std::cout << "Weight took " << weight_duration.count() << "s" << std::endl;
  std::cout << "Profile took " << profile_duration.count() << "s" << std::endl;
  this->orderModels();
  // iterating until convergence or reaching the max iteration
  conf.logger->info("Iteration 1 done, best model has a score of: " +
                    std::to_string(models[0].getScore()));
  for (int i = 1; i < conf.max_iterations; i++) {
    // std::cout << "iteration " << i << std::endl;
    // ** profiles initialization **

    // models are sorted in descending order by getScore()
    // re-initialize the worst half of the models
    for (int k = conf.model_batch_size - 1; k > conf.model_batch_size / 2 - 1;
         k--) {
      models[k].criteria.generateRandomCriteriaWeights();
      profileInitializer.initializeProfiles(models[k]);
      models[k].profiles.changeMode("alt");
      this->computeAccuracy(models[k]);
      std::cout << "accuracy after re-init: " << models[k].getScore()
                << std::endl;
    }

    // ** Weight and lambda update **
    for (int k = 0; k < conf.model_batch_size; k++) {
      weightUpdater.updateWeightsAndLambda(models[k]);
      this->computeAccuracy(models[k]);
      std::cout << "accuracy after weight update: " << models[k].getScore()
                << std::endl;
    }

    // ** Profiles update **
    for (int k = 0; k < conf.model_batch_size; k++) {
      profileUpdater.updateProfiles(models[k]);
      this->computeAccuracy(models[k]);
      std::cout << "accuracy after profile update: " << models[k].getScore()
                << std::endl;
    }

    this->orderModels();

    // if one model is accurately representing the dataset, stop the learning
    // algorithm
    conf.logger->info("Iteration " + std::to_string(i) +
                      " done, best model has a score of: " +
                      std::to_string(models[0].getScore()));
    if (models[0].getScore() == 1) {
      conf.logger->info(
          "Model with an getScore() of 1 found, stopping algorithm");
      break;
    }
    // std::cout << models[0].getScore() << std::endl;
  }
  conf.logger->info("Reaching max iteration, terminating the pipeline");
  for (int i = 0; i < std::max(int(models.size()), 5); i++) {
    std::cout << models[i].getScore() << std::endl;
  }
  return models[0];
}

void HeuristicPipeline::customSort() {
  std::vector<int> temp;
  for (int i = 0; i < models.size(); i++) {
    temp.push_back(i);
  }
  auto lambda_models = models;
  std::sort(temp.begin(), temp.end(),
            [lambda_models](const auto &a, const auto &b) {
              return lambda_models[a].getScore() > lambda_models[b].getScore();
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

void HeuristicPipeline::orderModels() {
  for (int k = 0; k < models.size(); k++) {
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
  model.setScore(float(acc) / float(altPerfs.getNumberAlt()));
}