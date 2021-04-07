#include <algorithm>
#include <sstream>
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

  // First iteration outside the loop: run every algorithm on all models
  // Creation of models and profile initialization
  conf.logger->info("Running 1st iteration on all models");

  using clock = std::chrono::system_clock;
  using sec = std::chrono::duration<double>;
  const auto before_init = clock::now();
  for (int k = 0; k < conf.model_batch_size; k++) {
    MRSortModel model = MRSortModel(n_cat, n_crit);
    profileInitializer.initializeProfiles(model);

    // change back to alt mode
    model.profiles.changeMode("alt");
    float acc_before = model.getScore();
    this->computeAccuracy(model);
    models.push_back(model);

    std::ostringstream ss;
    ss << "accuracy of model " << k << " after init: " << model.getScore()
       << ", gain of: " << model.getScore() - acc_before << std::endl;
    conf.logger->debug(ss.str());
  }
  const sec init_duration = clock::now() - before_init;
  const auto before_weight = clock::now();

  // Update weight and lambda
  for (int k = 0; k < conf.model_batch_size; k++) {
    weightUpdater.updateWeightsAndLambda(models[k]);
    float acc_before = models[k].getScore();
    this->computeAccuracy(models[k]);

    std::ostringstream ss;
    ss << "accuracy of model " << k
       << " after weight update: " << models[k].getScore()
       << ", gain of: " << models[k].getScore() - acc_before << std::endl;
    conf.logger->debug(ss.str());
  }
  const sec weight_duration = clock::now() - before_weight;
  const auto before_profile = clock::now();

  // Update profiles
  for (int k = 0; k < conf.model_batch_size; k++) {
    profileUpdater.updateProfiles(models[k]);
    float acc_before = models[k].getScore();
    this->computeAccuracy(models[k]);

    std::ostringstream ss;
    ss << "accuracy of model " << k
       << " after profile update: " << models[k].getScore()
       << ", gain of: " << models[k].getScore() - acc_before << std::endl;
    conf.logger->debug(ss.str());
  }

  const sec profile_duration = clock::now() - before_profile;
  auto total_time = init_duration.count() + weight_duration.count() +
                    profile_duration.count();
  std::ostringstream ss0;
  ss0 << "Profile initialization of all models took: " << init_duration.count()
      << "s"
      << " - " << int(100 * init_duration.count() / total_time) << "%"
      << std::endl;
  conf.logger->debug(ss0.str());
  std::ostringstream ss1;
  ss1 << "Weight update of all models took: " << weight_duration.count() << "s"
      << " - " << int(100 * weight_duration.count() / total_time) << "%"
      << std::endl;
  conf.logger->debug(ss1.str());
  std::ostringstream ss2;
  ss2 << "Profile update of all models took: " << profile_duration.count()
      << "s"
      << " - " << int(100 * profile_duration.count() / total_time) << "%"
      << std::endl;
  conf.logger->debug(ss2.str());
  this->orderModels();
  conf.logger->info("Iteration 1 done, best model has a score of: " +
                    std::to_string(models[0].getScore()));

  // iterating until convergence or reaching the max iteration
  for (int i = 1; i < conf.max_iterations; i++) {
    // ** profiles initialization **

    // models are sorted in descending order by getScore()
    // re-initialize the worst half of the models
    for (int k = conf.model_batch_size - 1; k > conf.model_batch_size / 2 - 1;
         k--) {
      models[k].criteria.generateRandomCriteriaWeights();
      profileInitializer.initializeProfiles(models[k]);
      models[k].profiles.changeMode("alt");
      float acc_before = models[k].getScore();
      this->computeAccuracy(models[k]);

      std::ostringstream ss;
      ss << "accuracy of model " << k
         << " after re-init: " << models[k].getScore()
         << ", gain of: " << models[k].getScore() - acc_before << std::endl;
      conf.logger->debug(ss.str());
    }
    // ** Weight and lambda update **
    for (int k = 0; k < conf.model_batch_size; k++) {
      weightUpdater.updateWeightsAndLambda(models[k]);
      float acc_before = models[k].getScore();
      this->computeAccuracy(models[k]);

      std::ostringstream ss;
      ss << "accuracy of model " << k
         << " after weight update: " << models[k].getScore()
         << ", gain of: " << models[k].getScore() - acc_before << std::endl;
      conf.logger->debug(ss.str());
    }
    // ** Profiles update **
    for (int k = 0; k < conf.model_batch_size; k++) {
      profileUpdater.updateProfiles(models[k]);
      float acc_before = models[k].getScore();
      this->computeAccuracy(models[k]);

      std::ostringstream ss;
      ss << "accuracy of model " << k
         << " after profile update: " << models[k].getScore()
         << ", gain of: " << models[k].getScore() - acc_before << std::endl;
      conf.logger->debug(ss.str());
    }
    this->orderModels();
    conf.logger->info("Iteration " + std::to_string(i) +
                      " done, best model has a score of: " +
                      std::to_string(models[0].getScore()));
    // if one model is accurately representing the dataset, stop the learning
    if (models[0].getScore() == 1) {
      conf.logger->info(
          "Model with an accuracy of 1 found, stopping algorithm");
      return models[0];
    }
  }
  conf.logger->info("Reaching max iteration, terminating the pipeline");
  for (int k = 0; k < std::max(int(models.size()), 5); k++) {
    std::ostringstream ss;
    ss << "accuracy of model " << k << ": " << models[k].getScore()
       << std::endl;
    conf.logger->debug(ss.str());
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
    if (e.second.rank_ == truth[e.first].rank_) {
      acc++;
    }
  }
  model.setScore(float(acc) / float(altPerfs.getNumberAlt()));
}