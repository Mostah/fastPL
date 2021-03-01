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

  auto cat_value_selector = [](auto pair) {
    return pair.second.getCategoryRank();
  };

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
  if (cat.getCategoryRank() == nbCategories - 1) {
    catAbove = cat.getCategoryRank();
    catBelow = catAbove - 1;
  } else {
    catBelow = cat.getCategoryRank();
    catAbove = catBelow + 1;
  }

  for (std::vector<Perf> vPerf : altPerformance_.getPerformanceTable()) {
    if (altPerformance_.getAlternativeAssignment(vPerf[0].getName())
                .getCategoryRank() == catBelow ||
        altPerformance_.getAlternativeAssignment(vPerf[0].getName())
                .getCategoryRank() == catAbove) {
      for (Perf p : vPerf) {
        if (p.getCrit() == crit.getId()) {
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
  float imaginaryProfilePerformance = perfAlt.getValue() + delta;
  // Creating 2 int that will count the number of correctly classified
  // alternatives for criterion crit for a profile performance of
  // imaginaryProfilePerformance.
  int catAboveCounter = 0;
  int catBelowCounter = 0;

  // candidates are computed thanks to the method
  // ProfileInitializer::getProfilePerformanceCandidates(crit, catBelow,
  // nbCategories);
  for (Perf can : candidates) {
    // if the performance of the candidate is higher than the profile
    // performance and that the alternatives category is catAbove then we have
    // correctly classified it
    if (can.getValue() > imaginaryProfilePerformance and
        altPerformance_.getAlternativeAssignment(can.getName())
                .getCategoryRank() == catAbove.getCategoryRank()) {
      ++catAboveCounter;
      // if the performance of the candidate is lower than the profile
      // performance and that the alternatives category is catBelow then we
      //  have correctly classified it
    } else if (can.getValue() < imaginaryProfilePerformance and
               altPerformance_.getAlternativeAssignment(can.getName())
                       .getCategoryRank() == catBelow.getCategoryRank()) {
      ++catBelowCounter;
    }
  }
  float proba;
  proba = catAboveCounter / catFrequency[catAbove.getCategoryRank()] +
          catBelowCounter / catFrequency[catBelow.getCategoryRank()];
  return proba;
}

std::vector<Perf> ProfileInitializer::initializeProfilePerformance(
    const Criterion &crit, Categories &categories,
    const std::vector<float> &catFre) {
  std::vector<float> categoryLimits;
  int nbCategories = categories.getNumberCategories();

  for (int i = 0; i < nbCategories - 1; i++) {
    std::vector<float> altProba;
    std::vector<Perf> candidates =
        ProfileInitializer::getProfilePerformanceCandidates(crit, categories[i],
                                                            nbCategories);
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

    categoryLimits.push_back(candidates[index].getValue());
  }
  std::reverse(categoryLimits.begin(), categoryLimits.end());
  int nbCategoryLimits = categoryLimits.size();
  // this is a work around since we would actually need to construct a
  // Performance with a categories object.
  std::vector<Perf> vect_p;
  for (int i = 0; i < nbCategoryLimits; i++) {
    // Cannot give a nice name to it since each vector of Perf need to have
    // same name
    vect_p.push_back(Perf(crit.getId(),
                          "cat" + std::to_string(nbCategoryLimits - 1 - i),
                          categoryLimits[i]));
  }
  return vect_p;
}

void ProfileInitializer::initializeProfiles(MRSortModel &model) {
  int nbIterations = 0;
  bool ordered = 0;
  while (!ordered) {
    std::cout << "iteration " << nbIterations << std::endl;
    ++nbIterations;
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
    PerformanceTable p = PerformanceTable(perf_vec);
    try {
      if (nbIterations % 10 == 0) {
        p.display();
      }
      Profiles p = Profiles(perf_vec, "crit");
      model.profiles = p;
      ordered = 1;
    } catch (std::invalid_argument const &err) {
    }
    if (nbIterations > 100) {
      throw std::domain_error("After 1000 intialization can't get an valid "
                              "(orderred) Profiles object ");
    }
  }
}
