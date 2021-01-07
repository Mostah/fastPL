#include "MRSort.h"
#include "Criteria.h"
#include "PerformanceTable.h"

#include <string>

MRSort::MRSort(Criteria &criteria, PerformanceTable &ap, float lambda,
               std::string id)
    : ap_(ap), criteria_(criteria) {
  lambda_ = lambda;
  id_ = id;
}

MRSort::~MRSort() {}

Criteria MRSort::getCriteria() const { return criteria_; }

PerformanceTable MRSort::getAp() const { return ap_; };

float MRSort::getLambda() const { return lambda_; }

std::string MRSort::getId() const { return id_; }

std::ostream &operator<<(std::ostream &out, const MRSort &mrsort) {
  out << "Model( id : " << mrsort.id_ << ", lambda : " << mrsort.lambda_
      << ", crit : " << mrsort.criteria_ << ", ap : " << mrsort.ap_ << ")";
  return out;
}