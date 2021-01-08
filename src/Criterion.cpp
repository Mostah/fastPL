#include "Criterion.h"
#include "utils.h"
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

void Criterion::generateDirection(unsigned long int seed){ 
    srand(seed);
    if ( ((float) rand() / RAND_MAX) < 0.5){ direction_ = -1; }
    else { direction_ = 1; }
}

void Criterion::generateWeight(unsigned long int seed){
    srand(seed);
    weight_ = (float) rand() / RAND_MAX ;
}



std::string Criterion::getId() const { return id_; }

void Criterion::setId(std::string id) { id_ = id; }

int Criterion::getDirection() const { return direction_; }

void Criterion::setDirection(int direction) { direction_ = direction; }

float Criterion::getWeight() const { return weight_; }

void Criterion::setWeight(float weight) { weight_ = weight; }

void Criterion::getRandomCriterionWeight(bool changeSeed) {
  if (changeSeed) {
    Criterion::setWeight(getRandomUniformNumber(1));
  } else {
    Criterion::setWeight(getRandomUniformNumber(0));
  }
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