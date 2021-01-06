#include "../include/Criterion.h"
#include <iostream>
#include <string>

Criterion::Criterion(std::string id) {
  id_ = id;
  name_ = "";
  direction_ = 1;
  weight_ = 0.;
}

Criterion::Criterion(std::string id, std::string name, int direction,
                     float weight) {
  id_ = id;
  name_ = name;
  direction_ = direction;
  weight_ = weight;
}

Criterion::Criterion(const Criterion &crit) {
  id_ = crit.getId();
  name_ = crit.getName();
  direction_ = crit.getDirection();
  weight_ = crit.getWeight();
}

std::string Criterion::getId() const { return id_; }

void Criterion::setId(std::string id) { id_ = id; }

std::string Criterion::getName() const { return name_; }

void Criterion::setName(std::string name) { name_ = name; }

int Criterion::getDirection() const { return direction_; }

void Criterion::setDirection(int direction) { direction_ = direction; }

float Criterion::getWeight() const { return weight_; }

void Criterion::setWeight(float weight) { weight_ = weight; }

std::ostream &operator<<(std::ostream &out, const Criterion &crit) {
  std::string dir = "";
  if (crit.direction_ == 1) {
    dir = "+";
  } else {
    dir = "-";
  }
  out << "Criterion(id : " << crit.id_ << ", name : " << crit.name_
      << ", direction : " << dir << ", weight : " << crit.weight_ << ")";
  return out;
}

Criterion::~Criterion() {}