#include "../../include/types/MRSortModel.h"
#include "../../include/types/Category.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/PerformanceTable.h"

#include <cstdlib>
#include <map>
#include <string>

MRSortModel::MRSortModel(Criteria &crits, PerformanceTable &profs, float lbd,
                         std::string id)
    : criteria(crits), profiles(profs) {
  if (!profiles.isOrdered()) {
    throw std::invalid_argument(
        "The given performance table cannot be use as a profile");
  }
  lambda = lbd;
  id_ = id;
}

MRSortModel::MRSortModel(int n_cat, int n_crit, std::string id)
    : criteria(n_crit), profiles(n_cat, criteria, "prof") {
  id_ = id;

  srand(time(NULL));
  lambda = ((float)rand() / RAND_MAX) / 2 + 0.5;

  criteria.generateRandomCriteriaWeights();

  profiles.generateRandomOrderedPerfValues();
}

std::vector<std::pair<std::string, std::string>>
MRSortModel::categoryAssignment(PerformanceTable &pt) {
  if (pt.getMode() != "alt") {
    throw std::invalid_argument(
        "Performance table set in wrong mode, should be alt.");
  }
  std::vector<std::pair<std::string, std::string>> cat_assignment;

  for (std::vector<Perf> alt : pt.getPerformanceTable()) {
    bool assigned = false;
    for (std::vector<Perf> profile_h : profiles.getPerformanceTable()) {
      float c = 0;
      for (Perf perf_j : alt) {
        for (Perf prof_i : profile_h) {
          if ((prof_i.getCrit() == perf_j.getCrit()) &&
              (perf_j.getValue() > prof_i.getValue())) {
            c = c + criteria[perf_j.getCrit()].getWeight();
          }
        }
      }
      if (c >= lambda) {
        cat_assignment.push_back(
            std::make_pair(alt[0].getName(), profile_h[0].getName()));
        assigned = true;
        break;
      }
    }
    if (!assigned) {
      cat_assignment.push_back(std::make_pair(alt[0].getName(), "base"));
    }
  }

  return cat_assignment;
}

MRSortModel::~MRSortModel() {}

std::string MRSortModel::getId() const { return id_; }

std::ostream &operator<<(std::ostream &out, const MRSortModel &mrsort) {
  out << "Model( id : " << mrsort.id_ << ", lambda : " << mrsort.lambda
      << ", crit : " << mrsort.criteria << ", profiles : " << mrsort.profiles
      << ")";
  return out;
}