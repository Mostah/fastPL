#include "../../extsrc/spdlog/include/spdlog/fmt/ostr.h"
#include "../../extsrc/spdlog/include/spdlog/spdlog.h"

#include "../../include/learning/ProfileInitializer.h"
#include "../../include/types/AlternativesPerformance.h"
#include "../../include/types/Categories.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/MRSortModel.h"
#include "../../include/types/Perf.h"
#include "../../include/utils.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

ProfileInitializer::ProfileInitializer(Config &config,
                                       AlternativesPerformance &altPerfs)
    : conf(config), altPerformance_(altPerfs) {
  conf.logger->debug("Starting ProfileInitializer object...");
}

ProfileInitializer::ProfileInitializer(const ProfileInitializer &profInit)
    : conf(profInit.conf), altPerformance_(profInit.altPerformance_) {
  conf.logger->debug("Starting ProfileInitializer object...");
}

AlternativesPerformance ProfileInitializer::getAlternativesPerformance() const {
  return altPerformance_;
}

void ProfileInitializer::setAlternativesPerformance(
    AlternativesPerformance &newAltPerfs) {
  altPerformance_ = newAltPerfs;
}

ProfileInitializer::~ProfileInitializer() {}

std::vector<float> ProfileInitializer::categoryFrequency() {

  // Extraction category rank from each alternative and storing it in values
  // SOMETIMES THIS DOESNT WORK this is seen as Segmentation Fault when using
  // InitProfile
  std::unordered_map<std::string, Category> map =
      altPerformance_.getAlternativesAssignments();
  std::vector<int> values(map.size());

  auto cat_value_selector = [](auto pair) { return pair.second.rank_; };

  std::transform(map.begin(), map.end(), values.begin(), cat_value_selector);

  // Counting the number of occurence for each category rank
  std::unordered_map<int, int> counts;
  for (auto v : values) {
    counts[v]++;
  }

  // Counting the number of alternatives in the altPerformance object
  const int nbAlternatives = std::accumulate(
      std::begin(counts), std::end(counts), 0,
      [](int value, const std::unordered_map<int, int>::value_type &p) {
        return value + p.second;
      });

  // Ordering the counts map
  std::map<int, int> orderedCounts(counts.begin(), counts.end());

  // Computing the frequency
  std::vector<float> frequency;
  for (auto const pair : orderedCounts) {
    float c = pair.second;
    frequency.push_back(c / nbAlternatives);
  }
  return frequency;
}

std::vector<Perf> ProfileInitializer::getProfilePerformanceCandidates(
    const Criterion &crit, const Category &cat, const int nbCategories) {

  int catAbove;
  int catBelow;
  std::vector<Perf> candidates;

  // Category rank starts at 0, worst category is 0
  if (cat.rank_ == nbCategories - 1) {
    catAbove = cat.rank_;
    catBelow = catAbove - 1;
  } else {
    catBelow = cat.rank_;
    catAbove = catBelow + 1;
  }

  std::unordered_map<std::string, Category> assig = altPerformance_.getAlternativesAssignments();
  for (std::vector<Perf> vPerf : altPerformance_.getPerformanceTable()) {
    if (assig[vPerf[0].name_].rank_ ==
            catBelow ||
        assig[vPerf[0].name_].rank_ ==
            catAbove) {
      for (Perf p : vPerf) {
        if (p.crit_ == crit.getId()) {
          candidates.push_back(p);
          break;
        }
      }
    }
  }
  return candidates;
}

float ProfileInitializer::weightedProbability(
    const Perf perfAlt, const Criterion &crit, const Category &catAbove,
    const Category &catBelow, const int nbCategories,
    const std::vector<float> &catFrequency, std::vector<Perf> &candidates,
    float delta) {
  // creating imaginary profile performance for criterion crit
  std::string critId = crit.getId();
  float imaginaryProfilePerformance = perfAlt.value_ + delta;
  // Creating 2 int that will count the number of correctly classified
  // alternatives for criterion crit for a profile performance of
  // imaginaryProfilePerformance.
  int catAboveCounter = 0;
  int catBelowCounter = 0;

  // candidates are computed thanks to the method
  // ProfileInitializer::getProfilePerformanceCandidates(crit, catBelow,
  // nbCategories);

  // TODO-OPTI this is creating new Perf that should ne be re-created, change
  // with index
  for (Perf &can : candidates) {
    // if the performance of the candidate is higher than the profile
    // performance and that the alternatives category is catAbove then we have
    // correctly classified it

    // TODO-OPTI getAlternativeAssignment should not be used
    if (can.value_ > imaginaryProfilePerformance and
        altPerformance_.getAlternativeAssignment(can.name_).rank_ ==
            catAbove.rank_) {
      ++catAboveCounter;
      // if the performance of the candidate is lower than the profile
      // performance and that the alternatives category is catBelow then we
      //  have correctly classified it
    } else if (can.value_ < imaginaryProfilePerformance and
               altPerformance_.getAlternativeAssignment(can.name_).rank_ ==
                   catBelow.rank_) {
      ++catBelowCounter;
    }
  }
  float proba;
  proba = catAboveCounter / catFrequency[catAbove.rank_] +
          catBelowCounter / catFrequency[catBelow.rank_];
  return proba;
}

std::vector<Perf> ProfileInitializer::initializeProfilePerformance(
    /*
    To optimize this function we would need to remove the while condition because
    the method loops through it too many times. One solution to this is to filter out 
    progressively candidates that have a smaller performance than the chosen performance for the
    profile at the time step before. For example if the value of profile of cat0 for crit0 is 0.3,
    then for cat0 crit1 you could only chose candidate values that are over 0.3. If there are no such candidate 
    you would have the start from scratch the elicitation of b0.
    */
    const Criterion &crit, Categories &categories,
    const std::vector<float> &catFre) {
  int nbCategories = categories.getNumberCategories();
  bool OrderedProfilePerformance = 0;
  std::vector<float> finalCategoryLimits;
  while (!OrderedProfilePerformance) {
    std::vector<float> categoryLimits;
    for (int i = 0; i < nbCategories - 1; i++) {
      std::vector<float> altProba;
      std::vector<Perf> candidates =
          ProfileInitializer::getProfilePerformanceCandidates(
              crit, categories[i], nbCategories);
      // OPTIM : POSSIBILITY parallelization synchrone
      for (Perf cand : candidates) {
        float proba = ProfileInitializer::weightedProbability(
            cand, crit, categories[i], categories[i + 1], nbCategories, catFre,
            candidates);
        altProba.push_back(proba);
      }

      std::random_device rd;
      float totProba = std::accumulate(altProba.begin(), altProba.end(), 0);
      float randomNumber = getRandomUniformInt(rd(), 0, totProba);
      float tmp = 0;
      int index = 0;
      for (int i = 0; i < candidates.size(); i++) {
        if (tmp < randomNumber) {
          tmp += altProba[i];
          index = i;
        } else {
          break;
        }
      }
      categoryLimits.push_back(candidates[index].value_);
    }
    if (std::is_sorted(categoryLimits.begin(), categoryLimits.end())) {
      OrderedProfilePerformance = 1;
      finalCategoryLimits = categoryLimits;
    }
  }
  std::reverse(finalCategoryLimits.begin(), finalCategoryLimits.end());
  int nbCategoryLimits = finalCategoryLimits.size();
  // this is a work around since we would actually need to construct a
  // Performance with a categories object.
  std::vector<Perf> vect_p;
  for (int i = 0; i < nbCategoryLimits; i++) {
    // Cannot give a nice name to it since each vector of Perf need to have
    // same name
    vect_p.push_back(Perf("b" + std::to_string(nbCategoryLimits - 1 - i),
                          crit.getId(), finalCategoryLimits[i]));
  }
  return vect_p;
}

void ProfileInitializer::initializeProfiles(MRSortModel &model) {
  std::vector<std::vector<Perf>> perf_vec;
  std::vector<Perf> firstAltPerf = altPerformance_.getPerformanceTable()[0];
  std::vector<std::string> criteriaIds = getCriterionIds(firstAltPerf);
  std::vector<float> catFreq = this->categoryFrequency();
  // CatFreq has a problem sometimes
  for (std::string criterion : criteriaIds) {
    // OPTIM : POSSIBILITY parallelization asynchrone
    std::vector<Perf> p = this->initializeProfilePerformance(
        criterion, model.categories, catFreq);
    std::reverse(p.begin(), p.end());
    perf_vec.push_back(p);
  }
  Profiles prof = Profiles(perf_vec, "crit");
  model.profiles = prof;
}
