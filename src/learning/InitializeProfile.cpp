#include "../../include/learning/InitializeProfile.h"
#include "../../include/types/AlternativesPerformance.h"
#include "../../include/utils.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

ProfileInitializer::ProfileInitializer(AlternativesPerformance &altPerfs)
    : altPerformance_(altPerfs) {}

ProfileInitializer::ProfileInitializer(const ProfileInitializer &profInit)
    : altPerformance_(profInit.getAlternativesPerformance()) {}

AlternativesPerformance ProfileInitializer::getAlternativesPerformance() const {
  return altPerformance_;
}

void ProfileInitializer::setAlternativesPerformance(
    AlternativesPerformance &newAltPerfs) {
  altPerformance_ = newAltPerfs;
}

std::vector<float> ProfileInitializer::categoryFrequency() {

  // Extraction category rank from each alternative and storing it in values
  std::unordered_map<std::string, Category> map =
      altPerformance_.getAlternativesAssignments();
  std::vector<int> values(map.size());

  auto cat_value_selector = [](auto pair) {
    return pair.second.getCategoryRank();
  };

  std::transform(map.begin(), map.end(), values.begin(), cat_value_selector);

  // Counting the number of occurence for each category rank
  std::unordered_map<int, int> counts;
  for (auto v : values)
    ++counts[v];

  // Counting the number of alternatives in the altPerformance object
  const int nbAlternatives = std::accumulate(
      std::begin(counts), std::end(counts), 0,
      [](int value, const std::unordered_map<int, int>::value_type &p) {
        return value + p.second;
      });

  // Computing the frequency
  std::vector<float> frequency;
  for (auto const pair : counts) {
    float c = pair.second;
    frequency.push_back(c / nbAlternatives);
  }

  return frequency;
}

std::vector<std::string> ProfileInitializer::getProfilePerformanceCandidates(
    const Criterion &crit, const Category &cat, const int nbCategories) {

  int catAbove;
  int catBelow;
  std::vector<std::string> candidates;

  // Category rank starts at 0
  if (cat.getCategoryRank() == nbCategories - 1) {
    catAbove = cat.getCategoryRank();
    catBelow = catAbove - 1;
  } else {
    catBelow = cat.getCategoryRank();
    catAbove = catBelow + 1;
  }

  for (auto pair : altPerformance_.getAlternativesAssignments()) {
    if (pair.second.getCategoryRank() == catBelow ||
        pair.second.getCategoryRank() == catAbove) {
      candidates.push_back(pair.first);
    }
  }
  return candidates;
}

float ProfileInitializer::weightedProbability(
    const std::string altId, const Criterion &crit, const Category &catAbove,
    const Category &catBelow, const int nbCategories,
    const std::vector<float> &catFrequency, float delta) {
  // creating imaginary profile performance for criterion crit
  std::string critId = crit.getId();
  float imaginaryProfilePerformance =
      altPerformance_.getPerf(altId, critId).getValue() + delta;
  std::vector<std::string> candidates =
      ProfileInitializer::getProfilePerformanceCandidates(crit, catBelow,
                                                          nbCategories);
  // Creating 2 int that will count the number of correctly classified
  // alternatives for criterion crit for a profile performance of
  // imaginaryProfilePerformance.
  int catAboveCounter = 0;
  int catBelowCounter = 0;

  for (std::string can : candidates) {
    // if the performance of the candidate is higher than the profile
    // performance and that the alternatives category is catAbove then we have
    // correctly classified it
    if (altPerformance_.getPerf(can, critId).getValue() >
            imaginaryProfilePerformance and
        altPerformance_.getAlternativeAssignment(can) == catAbove) {
      ++catAboveCounter;
      // if the performance of the candidate is lower than the profile
      // performance and that the alternatives category is catBelow then we have
      // correctly classified it
    } else if (altPerformance_.getPerf(can, critId).getValue() <
                   imaginaryProfilePerformance and
               altPerformance_.getAlternativeAssignment(can) == catBelow) {
      ++catBelowCounter;
    }
  }
  float proba;
  proba = catAboveCounter / catFrequency[catAbove.getCategoryRank()] +
          catBelowCounter / catFrequency[catBelow.getCategoryRank()];
  return proba;
}

std::vector<float> ProfileInitializer::initializeProfilePerformance(
    Criterion &crit, Categories const &categories,
    const std::vector<float> &catFre) {
  std::vector<float> CategoryLimits;
  int nbCategories = categories.size();

  for (int i = 0; i < nbCategories - 1; i++) {
    std::vector<float> altProba;
    std::vector<std::string> candidates =
        ProfileInitializer::getProfilePerformanceCandidates(
            criterion, categories[i], nbCategories);
    for (std::string cand : candidates) {
      float proba = ProfileInitializer::weightedProbability(
          cand.crit, categories[i], categories[i + 1], nbCategories, catFre);
      altProba.push_back(proba);
    }
    float totProba = accumulate(altProba.begin(), altProba.end(), 0);
  }
}