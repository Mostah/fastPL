#include "../../include/types/Perf.h"
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
  name_ = perf.name_;
  crit_ = perf.crit_;
  value_ = perf.value_;
}

Perf::~Perf() {}

std::ostream &operator<<(std::ostream &out, const Perf &p) {
  out << "Perf( name : " << p.name_ << ", crit : " << p.crit_
      << ", value : " << p.value_ << " )";
  return out;
}

bool Perf::operator==(const Perf &perf2) const {
  return (this->crit_ == perf2.crit_ && this->value_ == perf2.value_ &&
          this->name_ == perf2.name_);
}