#include "../../include/learning/ProfileUpdater.h"
#include "../../include/utils.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

ProfileUpdater::ProfileUpdater(Config &conf,
                               AlternativesPerformance &altPerf_data,
                               float epsilon)
    : conf(conf), altPerf_data(altPerf_data), epsilon_(epsilon) {
  conf.logger->debug("Starting ProfileUpdater object...");
}

ProfileUpdater::ProfileUpdater(const ProfileUpdater &profUp)
    : conf(profUp.conf), altPerf_data(profUp.altPerf_data),
      epsilon_(profUp.epsilon_) {
  conf.logger->debug("Starting ProfileUpdater object...");
}

ProfileUpdater::~ProfileUpdater() {}

std::unordered_map<float, float> ProfileUpdater::computeAboveDesirability(
    MRSortModel &model, std::string critId, Perf &b, Perf &b_above,
    Category &cat, Category &cat_above,
    std::unordered_map<std::string, float> &ct_prof,
    AlternativesPerformance &altPerf_model) {
  // Data from the problem
  float lambda = model.lambda;
  float weight = model.criteria[critId].getWeight();
  float epsilon = this->epsilon_;

  // Alternatives between given profile and above profile. AltPerf_model needs
  // to be sorted and in mode crit to have a meaniful iteration in the loop
  altPerf_model.sort();
  std::vector<Perf> alt_between =
      altPerf_model.getAltBetween(critId, b.getValue(), b_above.getValue());
  // Initializing the map of desirability indexes
  std::unordered_map<float, float> desirability_above;
  float numerator = 0;
  float denominator = 0;

  for (Perf alt : alt_between) {
    // Checking if the move will not go above b_above
    if (alt.getValue() + epsilon < b_above.getValue()) {
      std::string altName = alt.getName();
      float conc = ct_prof[altName];
      float diff = conc - weight;
      int aa_data =
          altPerf_data.getAlternativeAssignment(altName).getCategoryRank();
      int aa_model =
          altPerf_model.getAlternativeAssignment(altName).getCategoryRank();

      // Here we are checking if the move of profile b right above/under the
      // performance of alt is going to help the model
      if (aa_data == cat_above.getCategoryRank()) {
        // Correct classification
        // Moving the profile results in misclassification -> Q
        if (aa_model == cat_above.getCategoryRank() and diff < lambda) {
          denominator += 5;
        }
        // Wrong classification
        // Moving the profile is in favor of wrong classification -> R
        else if (aa_model == cat.getCategoryRank()) {
          denominator += 1;
        }
      } else if (aa_data == cat.getCategoryRank() and
                 aa_model == cat_above.getCategoryRank()) {
        // Wrong classification
        // Moving the profile is in favor of right classification -> W
        if (diff >= lambda) {
          numerator += 0.5;
          denominator += 1;
          desirability_above[alt.getValue() + epsilon] =
              numerator / denominator;
        }
        // Wrong classification
        // Moving the profile results in correct classification -> V
        else {
          numerator += 2;
          denominator += 1;
          desirability_above[alt.getValue() + epsilon] =
              numerator / denominator;
        }
      }
      // Wrong classification
      // Moving the profile is in favor of right classification -> T
      else if (aa_data != aa_model and
               altPerf_model.getAlternativeAssignment(altName)
                       .getCategoryRank() >= cat_above.getCategoryRank() and
               altPerf_data.getAlternativeAssignment(altName)
                       .getCategoryRank() < cat.getCategoryRank()) {
        numerator += 0.1;
        denominator += 1;
        desirability_above[alt.getValue() + epsilon] = numerator / denominator;
      }
    }
  }
  return desirability_above;
}

std::unordered_map<float, float> ProfileUpdater::computeBelowDesirability(
    MRSortModel &model, std::string critId, Perf &b, Perf &b_below,
    Category &cat, Category &cat_above,
    std::unordered_map<std::string, float> &ct_prof,
    AlternativesPerformance &altPerf_model) {
  // Data from the problem
  float lambda = model.lambda;
  float weight = model.criteria[critId].getWeight();
  // Direction & epsilon not in original algorithm
  float epsilon = this->epsilon_;

  // Alternatives between given profile and above profile. AltPerf_model needs
  // to be sorted and in mode crit to have a meaniful iteration in the loop
  altPerf_model.sort();
  std::vector<Perf> alt_between =
      altPerf_model.getAltBetween(critId, b_below.getValue(), b.getValue());
  // Initializing the map of desirability indexes
  std::unordered_map<float, float> desirability_below;
  float numerator = 0;
  float denominator = 0;

  // Going in reverse order as we are moving the profile down
  for (int i = alt_between.size() - 1; i >= 0; i--) {
    Perf alt = alt_between[i];
    // Checking if the move will not go below b_below
    if ((alt.getValue() - epsilon) > b_below.getValue()) {
      std::string altName = alt.getName();
      float conc = ct_prof[altName];
      float diff = conc + weight;

      int aa_data =
          altPerf_data.getAlternativeAssignment(altName).getCategoryRank();
      int aa_model =
          altPerf_model.getAlternativeAssignment(altName).getCategoryRank();

      // Here we are checking if the move of profile b at the level of the
      // performance of alt is going to help the model
      if (aa_data == cat_above.getCategoryRank() and
          aa_model == cat.getCategoryRank()) {
        // Wrong classification
        // Moving the profile results in correct classification -> V
        if (diff >= lambda) {
          numerator += 2;
          denominator += 1;
          desirability_below[alt.getValue() - epsilon] =
              numerator / denominator;
        }
        // Wrong classification
        // Moving the profile is in favor of right classification -> W
        else {
          numerator += 0.5;
          denominator += 1;
          desirability_below[alt.getValue() - epsilon] =
              numerator / denominator;
        }
      } else if (aa_data == cat.getCategoryRank()) {
        // Correct classification
        // Moving the profile results in misclassification -> Q
        if (aa_model == cat.getCategoryRank() and diff >= lambda) {
          denominator += 5;
        }
        // Wrong classification
        // Moving the profile is in favor of wrong classification -> R
        else if (aa_model == cat_above.getCategoryRank()) {
          denominator += 1;
        }
      }
      // Wrong classification
      // Moving the profile is in favor of right classification -> T
      else if (aa_data != aa_model and
               altPerf_model.getAlternativeAssignment(altName)
                       .getCategoryRank() > cat.getCategoryRank() and
               altPerf_data.getAlternativeAssignment(altName)
                       .getCategoryRank() > cat.getCategoryRank()) {
        numerator += 0.1;
        denominator += 1;
        desirability_below[alt.getValue() - epsilon] = numerator / denominator;
      }
    }
  }
  return desirability_below;
}

std::pair<float, float> ProfileUpdater::chooseMaxDesirability(
    std::unordered_map<float, float> &desirability, Perf &b) {
  float key_max = 0;
  float value_max = 0;
  for (auto it = desirability.begin(); it != desirability.end(); ++it) {
    if (it->second > value_max) {
      key_max = it->first;
      value_max = it->second;
    }
  }
  return std::make_pair(key_max, value_max);
}

void ProfileUpdater::updateTables(
    MRSortModel &model, std::string critId, Perf &b_old, Perf &b_new,
    std::unordered_map<std::string, std::unordered_map<std::string, float>> &ct,
    AlternativesPerformance &altPerf_model) {
  if (b_old.getName() != b_new.getName() ||
      b_old.getCrit() != b_new.getCrit()) {
    throw std::invalid_argument("Profile perfs must have same name and crit");
  }

  float w = 0;
  // Determine if the profile is moving up or down to know how to adjust the
  // weight for the concordance and calculate alternatives between old profile
  // perf and new profile perf.
  std::vector<Perf> alt_between;
  if (b_old.getValue() > b_new.getValue()) {
    w = model.criteria[critId].getWeight();
    alt_between =
        altPerf_model.getAltBetween(critId, b_new.getValue(), b_old.getValue());
  } else {
    w = -model.criteria[critId].getWeight();
    alt_between =
        altPerf_model.getAltBetween(critId, b_old.getValue(), b_new.getValue());
  }

  for (Perf alt : alt_between) {
    // Data assignment
    std::string aa_data =
        altPerf_data.getAlternativeAssignment(alt.getName()).getCategoryId();
    // Old assignmment
    std::string aa_old =
        altPerf_model.getAlternativeAssignment(alt.getName()).getCategoryId();

    // Update concordance table
    float c = ct[b_old.getName()][alt.getName()] + w;
    ct[b_old.getName()][alt.getName()] = c;
    // Update profile
    model.profiles.setPerf(b_new.getName(), b_new.getCrit(), b_new.getValue());

    // New assignment
    altPerf_model.changeMode("alt");
    auto alternative = altPerf_model.operator[](alt.getName());
    std::vector<std::vector<Perf>> pt = model.profiles.getPerformanceTable();
    Category cat_new = model.categoryAssignment(alternative, pt);
    std::string aa_new = cat_new.getCategoryId();

    // Update alternative assignment
    altPerf_model.setAlternativeAssignment(alt.getName(), cat_new);

    // Update model score
    int n_alt = altPerf_data.getNumberAlt();
    float change = static_cast<float>(1) / static_cast<float>(n_alt);
    if (aa_old == aa_new) {
      break;
    } else if (aa_old == aa_data) {
      model.setScore(model.getScore() - change);
    } else if (aa_new == aa_data) {
      model.setScore(model.getScore() + change);
    }
  }
}

void ProfileUpdater::optimizeProfile(
    std::vector<Perf> &prof, Category &cat_below, Category &cat_above,
    MRSortModel &model,
    std::unordered_map<std::string, std::unordered_map<std::string, float>> &ct,
    AlternativesPerformance &altPerf_model) {
  // get the worst and best values in the dataset to compute the boundaries of
  // the profile
  std::pair<float, float> bounds = altPerf_model.getBoundaries();
  std::pair<std::vector<Perf>, std::vector<Perf>> below_above =
      model.profiles.getBelowAndAboveProfile(prof[0].getName(), bounds.first,
                                             bounds.second);
  std::vector<Perf> prof_below = below_above.first;
  std::vector<Perf> prof_above = below_above.second;

  for (Criterion crit : model.criteria.getCriterionVect()) {
    Perf b = getPerfOfCrit(prof, crit.getId());
    Perf b_below = getPerfOfCrit(prof_below, crit.getId());
    Perf b_above = getPerfOfCrit(prof_above, crit.getId());

    std::unordered_map<std::string, float> ct_prof = ct[b.getName()];
    std::unordered_map<float, float> below_des = this->computeBelowDesirability(
        model, crit.getId(), b, b_below, cat_below, cat_above, ct_prof,
        altPerf_model);
    std::unordered_map<float, float> above_des = this->computeAboveDesirability(
        model, crit.getId(), b, b_above, cat_below, cat_above, ct_prof,
        altPerf_model);
    std::unordered_map<float, float> desirability = below_des;
    desirability.insert(above_des.begin(), above_des.end());

    std::pair<float, float> max = this->chooseMaxDesirability(desirability, b);
    float key_max = max.first;
    float value_max = max.second;

    if (value_max != 0) {
      float r = getRandomUniformFloat();
      if (r <= value_max) {
        Perf b_new = Perf(b);
        b_new.setValue(key_max);
        this->updateTables(model, crit.getId(), b, b_new, ct, altPerf_model);
      }
    }
  }
}

void ProfileUpdater::optimize(
    MRSortModel &model,
    std::unordered_map<std::string, std::unordered_map<std::string, float>> &ct,
    AlternativesPerformance &altPerf_model) {
  if (model.profiles.getMode() != "alt") {
    model.profiles.changeMode("alt");
  }
  if (!model.profiles.isProfileOrdered()) {
    throw std::invalid_argument("Profile table is not ordered");
  }
  int i = 0;
  for (std::vector<Perf> profile : model.profiles.getPerformanceTable()) {
    Category cat_below = model.categories.getCategoryOfRank(i);
    Category cat_above = model.categories.getCategoryOfRank(i + 1);
    this->optimizeProfile(profile, cat_below, cat_above, model, ct,
                          altPerf_model);
    i = i + 1;
  };
}

void ProfileUpdater::updateProfiles(MRSortModel &model) {
  std::unordered_map<std::string, std::unordered_map<std::string, float>> ct =
      model.computeConcordanceTable(altPerf_data);
  AlternativesPerformance altPerf_model =
      model.categoryAssignments(altPerf_data);
  this->optimize(model, ct, altPerf_model);
}
