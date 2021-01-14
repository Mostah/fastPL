#include "../../include/types/Criteria.h"
#include "../../include/types/Criterion.h"
#include "../../include/utils.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

Criteria::Criteria(std::vector<Criterion> &criterion_vect) {
  std::vector<std::string> crit_id_vect;
  for (Criterion crit : criterion_vect) {
    // ensure there is no criterion with duplicated name
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

std::vector<float> Criteria::getWeights() const {
  std::vector<float> weights;
  for (Criterion c : criterion_vect_) {
    weights.push_back(c.getWeight());
  }
  return weights;
}

void Criteria::setWeights(std::vector<float> newWeigths) {
  if (newWeigths.size() != criterion_vect_.size()) {
    throw std::invalid_argument(
        "New weight vector must have same length as Criteria ie have the same "
        "value as the number of criteria");
  }
  for (int i = 0; i < criterion_vect_.size(); i++) {
    criterion_vect_[i].setWeight(newWeigths[i]);
  }
}

void Criteria::normalizeWeights() {
  float sum = Criteria::getSumWeight();
  std::vector<float> weights = Criteria::getWeights();
  std::transform(weights.begin(), weights.end(), weights.begin(),
                 [&sum](float &c) { return c / sum; });
  for (int i = 0; i < weights.size(); i++) {
    criterion_vect_[i].setWeight(weights[i]);
  }
}

void Criteria::generateRandomCriteriaWeights(bool changeSeed) {
  std::vector<float> weights;
  for (int i = 0; i < criterion_vect_.size(); i++) {
    weights.push_back(getRandomUniformNumber(changeSeed));
  }
  float totSum = std::accumulate(weights.begin(), weights.end(), 0.00f);
  std::transform(weights.begin(), weights.end(), weights.begin(),
                 [totSum](float &c) { return c / totSum; });
  Criteria::setWeights(weights);
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