#include "../../include/types/Profiles.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

Profiles::Profiles(std::vector<Performance> &perf_vect)
    : PerformanceTable(perf_vect) {
  if (!this->isProfileOrdered()) {
    throw std::invalid_argument(
        "The given performance vector cannot be used as a profiles performance "
        "table. Each row must be ranked such that for each row i we have on "
        "each criterion j : v_i - 1_j > v_i_j > v_i + 1_j");
  }
}

Profiles::Profiles(int nb_of_prof, Criteria &crits, std::string prefix)
    : PerformanceTable(nb_of_prof, crits, prefix) {
  this->generateRandomPerfValues();
}

Profiles::Profiles(const Profiles &profiles) : PerformanceTable(profiles) {}

Profiles::~Profiles() {}

std::ostream &operator<<(std::ostream &out, const Profiles &profs) {
  out << "Profiles[ ";
  for (std::vector<Perf> p : profs.pt_) {
    out << "Profile: ";
    for (Perf perf : p) {
      out << perf << " ";
    }
    out << "| ";
  }
  out << "]";
  return out;
}

void Profiles::generateRandomPerfValues(unsigned long int seed, int lower_bound,
                                        int upper_bound) {
  if (lower_bound > upper_bound) {
    throw std::invalid_argument(
        "Lower bound must be lower than the upper bound.");
  }
  srand(seed);
  for (int j = 0; j < pt_[0].size(); j++) {
    std::vector<float> r_vect;
    for (int i = 0; i < pt_.size(); i++) {
      r_vect.push_back(getRandomUniformFloat(seed, lower_bound, upper_bound));
    }
    std::sort(r_vect.begin(), r_vect.end());
    std::reverse(r_vect.begin(), r_vect.end());
    for (int i = 0; i < pt_.size(); i++) {
      pt_[i][j].setValue(r_vect[i]);
    }
  }
}

bool Profiles::isProfileOrdered() {
  for (int i = 0; i < pt_.size() - 1; i++) {
    for (int j = 0; j < pt_[i].size(); j++) {
      if (pt_[i][j].getValue() < pt_[i + 1][j].getValue()) {
        return false;
      }
    }
  }
  return true;
}