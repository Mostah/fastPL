#include "CriterionValue.h"
#include <iostream>

// Constructors definitions
CriterionValue::CriterionValue(std::string id, float weight) {
  id_ = id;
  weight_ = weight;
}

CriterionValue::CriterionValue(std::string id) {
  id_ = id;
  weight_ = 0.;
}

CriterionValue::CriterionValue(const CriterionValue &cv) {
  id_ = cv.getId();
  weight_ = cv.getWeight();
}

CriterionValue::~CriterionValue() {}

std::string CriterionValue::getId() const { return id_; }

void CriterionValue::setId(std::string id) { id_ = id; }

float CriterionValue::getWeight() const { return weight_; }

void CriterionValue::setWeight(float weight) { weight_ = weight; }

// Print operator
std::ostream &operator<<(std::ostream &out, const CriterionValue &cv) {
  out << "CriterionValue(" << cv.id_ << ": " << cv.weight_ << ")";
  return out;
}