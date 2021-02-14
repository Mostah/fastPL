#include "../../include/types/Profiles.h"
#include "../../include/types/Perf.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

Profiles::Profiles(std::vector<std::vector<Perf>> &perf_vect, std::string mode)
    : PerformanceTable(perf_vect) {
  if (mode != "alt" || model != "crit") {
    throw std::invalid_argument("Invalid Profile mode given.");
  }
  mode_ = mode;
  if (!this->isProfileOrdered()) {
    throw std::invalid_argument(
        "The given performance vector cannot be used as a profiles performance "
        "table. Each row must be ranked such that for each row i we have on "
        "each criterion j : v_i - 1_j > v_i_j > v_i + 1_j");
  }
}

Profiles::Profiles(int nb_of_prof, Criteria &crits, std::string mode,
                   std::string prefix)
    : PerformanceTable(nb_of_prof, crits, prefix) {
  this->generateRandomPerfValues();
  if (mode != "alt" || model != "crit") {
    throw std::invalid_argument("Invalid Profile mode given.");
  }
  mode_ = mode;
  sorted_ = 1;
}

Profiles::Profiles(const Profiles &profiles) : PerformanceTable(profiles) {
  mode_ = profiles.getMode();
  sorted_ = 1;
}

std::ostream &operator<<(std::ostream &out, const Profiles &profs) {
  out << "Profiles[ " << std::endl;
  for (std::vector<Perf> p : profs.pt_) {
    out << "Profile: ";
    for (Perf perf : p) {
      out << perf << " ";
    }
    out << "| " << std::endl;
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

  int nbProfiles = pt_.size();

  std::random_device rd;
  for (int j = 0; j < pt_[0].size(); j++) {
    std::vector<float> r_vect;
    for (int i = 0; i < pt_.size(); i++) {
      r_vect.push_back(getRandomUniformFloat(rd(), lower_bound, upper_bound));
    }
    std::sort(r_vect.begin(), r_vect.end());
    std::reverse(r_vect.begin(), r_vect.end());
    for (int i = 0; i < nbProfiles; i++) {
      pt_[nbProfiles - 1 - i][j].setValue(r_vect[i]);
    }
  }
}

bool Profiles::isProfileOrdered() {
  if (mode_ == "crit") {
    for (int crit = 0; crit < pt_.size(); crit++) {
      for (int catLimit = 0; catLimit < pt_[crit].size() - 1; catLimit++) {
        if (pt_[crit][catLimit].getValue() >
            pt_[crit][catLimit + 1].getValue()) {
          return false;
        }
      }
    }
    return true;
  } else {
    for (int profile = 0; profile < pt_.size() - 1; profile++) {
      for (int crit = 0; crit < pt_[profile].size(); crit++) {
        if (pt_[profile][crit].getValue() > pt_[profile + 1][crit].getValue()) {
          return false;
        }
      }
    }
    return true;
  }
}
