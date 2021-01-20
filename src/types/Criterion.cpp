#include "../../include/types/Criterion.h"
#include "../../include/utils.h"
#include <iostream>
#include <string>

Criterion::Criterion(std::string id, int direction, float weight) {
  id_ = id;
  direction_ = direction;
  weight_ = weight;
}

Criterion::Criterion(const Criterion &crit) {
  id_ = crit.getId();
  direction_ = crit.getDirection();
  weight_ = crit.getWeight();
}

std::string Criterion::getId() const { return id_; }

void Criterion::setId(std::string id) { id_ = id; }

int Criterion::getDirection() const { return direction_; }

void Criterion::setDirection(int direction) { direction_ = direction; }

float Criterion::getWeight() const { return weight_; }

void Criterion::setWeight(float weight) { weight_ = weight; }

void Criterion::generateDirection(unsigned long int seed) {
  float f = getRandomUniformFloat();
  if (f < 0.5) {
    direction_ = -1;
  } else {
    direction_ = 1;
  }
}

void Criterion::generateWeight(unsigned long int seed) {
  weight_ = getRandomUniformFloat(seed);
}

std::ostream &operator<<(std::ostream &out, const Criterion &crit) {
  std::string dir = "";
  if (crit.direction_ == 1) {
    dir = "+";
  } else {
    dir = "-";
  }
  out << "Criterion(id : " << crit.id_ << ", direction : " << dir
      << ", weight : " << crit.weight_ << ")";
  return out;
}

Criterion::~Criterion() {}