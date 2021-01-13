#include "MRSortModel.h"
#include "Criteria.h"
#include "PerformanceTable.h"

#include <string>

MRSortModel::MRSortModel(Criteria &crits, PerformanceTable &profs, float lbd,
                         std::string id)
    : criteria(crits), profiles(profs) {
  if (!profiles.isSorted()) {
    profiles.sort();
  }
  lambda = lbd;
  id_ = id;
}

MRSortModel::MRSortModel(int n_cat, int n_crit, std::string id = "model") {
  id_ = id;
  criteria = Criteria(n_crit);
  criteria.profiles
}

MRSortModel::~MRSortModel() {}

std::string MRSortModel::getId() const { return id_; }

std::ostream &operator<<(std::ostream &out, const MRSortModel &mrsort) {
  out << "Model( id : " << mrsort.id_ << ", lambda : " << mrsort.lambda
      << ", crit : " << mrsort.criteria << ", profiles : " << mrsort.profiles
      << ")";
  return out;
}