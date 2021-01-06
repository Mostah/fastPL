#include "../include/Criteria.h"
#include "../include/Criterion.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

Criteria::Criteria(std::vector<Criterion> &criterion_vect) {
  std::vector<std::string> crit_id_vect;
  for (Criterion crit : criterion_vect) {
    // ensure there is no criterion with dupplicated name
    if (std::find(crit_id_vect.begin(), crit_id_vect.end(), crit.getId()) !=
        crit_id_vect.end()) {
      throw std::invalid_argument("Each criterion must have different ids.");
    }
    crit_id_vect.push_back(crit.getId());

    criterion_vect_.push_back(Criterion(crit));
  }
}

Criteria::Criteria(int nb_of_criteria, std::string prefix) {
  for (int i = 0; i < nb_of_criteria; i++) {
    criterion_vect_.push_back(Criterion(prefix + std::to_string(i)));
  }
}

Criteria::Criteria(const Criteria &crits) {
  // deep copy
  for (int i = 0; i < crits.criterion_vect_.size(); i++) {
    criterion_vect_.push_back(Criterion(crits.criterion_vect_[i]));
  }
}

Criteria::~Criteria() {}

std::ostream &operator<<(std::ostream &out, const Criteria &crits) {
  out << "Criteria(";
  for (Criterion crit : crits.criterion_vect_) {
    out << crit << ", ";
  }
  out << ")";
  return out;
}

void Criteria::setCriterionVect(std::vector<Criterion> &criterion_vect) {
  criterion_vect_.clear();
  // deep copy
  for (int i = 0; i < criterion_vect.size(); i++) {
    criterion_vect_.push_back(Criterion(criterion_vect[i]));
  }
}

std::vector<Criterion> Criteria::getCriterionVect() const {
  return criterion_vect_;
};

float Criteria::getMinWeight() {
  if (criterion_vect_.size() == 0) {
    return 0;
  }
  float min = criterion_vect_[0].getWeight();
  for (Criterion crit : criterion_vect_) {
    if (crit.getWeight() < min) {
      min = crit.getWeight();
    }
  }
  return min;
}

float Criteria::getMaxWeight() {
  if (criterion_vect_.size() == 0) {
    return 0;
  }
  float max = criterion_vect_[0].getWeight();
  for (Criterion crit : criterion_vect_) {
    if (crit.getWeight() > max) {
      max = crit.getWeight();
    }
  }
  return max;
}
float Criteria::getSumWeight() {
  float sum = 0;
  for (Criterion crit : criterion_vect_) {
    sum += crit.getWeight();
  }
  return sum;
}

Criterion Criteria::operator[](std::string name) const {
  for (Criterion c : criterion_vect_) {
    if (c.getId() == name) {
      return c;
    }
  }
  throw std::invalid_argument("Criterion not found in this Criteria vector");
}

Criterion Criteria::operator[](int index) { return criterion_vect_[index]; }
Criterion Criteria::operator[](int index) const {
  return criterion_vect_[index];
}