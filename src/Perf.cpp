#include "../include/Perf.h"
#include <iostream>
#include <string>

Perf::Perf(std::string name, std::string criterion, float value) {
  name_ = name;
  crit_ = criterion;
  value_ = value;
}

Perf::Perf(std::string name, std::string criterion) {
  name_ = name;
  crit_ = criterion;
  value_ = 0;
}

Perf::Perf(const Perf &perf) {
  name_ = perf.getName();
  crit_ = perf.getCrit();
  value_ = perf.getValue();
}

Perf::~Perf() {}

std::ostream &operator<<(std::ostream &out, const Perf &p) {
  out << "Perf( name : " << p.getName() << ", crit : " << p.getCrit()
      << ", value : " << p.getValue() << " )";
  return out;
}

std::string Perf::getName() const { return name_; }

void Perf::setName(std::string name) { name_ = name; }

float Perf::getValue() const { return value_; }

void Perf::setValue(float value) { value_ = value; }

std::string Perf::getCrit() const { return crit_; }

void Perf::setCrit(std::string crit) { crit_ = crit; }
