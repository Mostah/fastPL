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
    : PerformanceTable(perf_vect, mode) {
  mode_ = mode;
  if (!this->isProfileOrdered()) {
    throw std::invalid_argument("Profile in its given mode is not ordered");
  }
}

Profiles::Profiles(int nb_of_prof, Criteria &crits, std::string mode,
                   std::string prefix)
    : PerformanceTable(nb_of_prof, crits, prefix) {
  if (mode != "alt" and mode != "crit") {
    throw std::invalid_argument("Invalid Profile mode given.");
  }
  if (mode == "crit") {
    this->changeMode("crit");
  }
  this->generateRandomPerfValues();
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

  if (mode_ == "alt") {
    int nbProfiles = pt_.size();
    std::random_device rd;
    for (int j = 0; j < pt_[0].size(); j++) {
      std::vector<float> r_vect;
      for (int i = 0; i < nbProfiles; i++) {
        r_vect.push_back(getRandomUniformFloat(rd(), lower_bound, upper_bound));
      }
      std::sort(r_vect.begin(), r_vect.end());
      std::reverse(r_vect.begin(), r_vect.end());
      for (int k = 0; k < nbProfiles; k++) {
        pt_[nbProfiles - 1 - k][j].setValue(r_vect[k]);
      }
    }
  } else {
    int nbCrit = pt_.size();
    int nbCat = pt_[0].size();
    std::random_device rd;
    for (int j = 0; j < nbCrit; j++) {
      std::vector<float> r_vect;
      for (int i = 0; i < nbCat; i++) {
        r_vect.push_back(getRandomUniformFloat(rd(), lower_bound, upper_bound));
      }
      std::sort(r_vect.begin(), r_vect.end());
      for (int k = 0; k < nbCat; k++) {
        pt_[j][k].setValue(r_vect[k]);
      }
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
  return true;
}

std::pair<std::vector<Perf>, std::vector<Perf>>
Profiles::getBelowAndAboveProfile(std::string profName) {
  int n_crit = this->getNumberCrit();
  std::vector<Perf> base;
  for (int i = 0; i < n_crit; i++) {
    base.push_back(Perf("base", pt_[0][i].getCrit(), 0));
  }
  std::vector<Perf> top;
  for (int i = 0; i < n_crit; i++) {
    top.push_back(Perf("top", pt_[0][i].getCrit(), 1));
  }

  std::vector<Perf> below;
  std::vector<Perf> above;
  if (mode_ == "alt") {
    for (int h = 0; h < pt_.size(); h++) {
      if (pt_[h][0].getName() == profName) {
        if (h == 0) {
          below = base;
          above = pt_[h + 1];
          return std::make_pair(below, above);
        } else if (h == pt_.size() - 1) {
          below = pt_[h - 1];
          above = top;
          return std::make_pair(below, above);
        } else {
          below = pt_[h - 1];
          above = pt_[h + 1];
          return std::make_pair(below, above);
        }
      }
    }
    throw std::invalid_argument("Profile not found.");
  } else {
    throw std::invalid_argument("Profiles perftable mode should be crit.");
  }
}

void Profiles::setPerf(std::string name, std::string crit, float value) {
  // supposing the pt is consistent:
  // if in mode alt, each row contains 1 and only 1 (alternative or profile)
  // if in mode crit, each row contains 1 and only 1 criterion
  if (mode_ == "alt") {
    for (std::vector<Perf> &p : pt_) {
      if (p[0].getName() == name) {
        for (Perf &perf : p) {
          if (perf.getCrit() == crit) {
            perf.setValue(value);
            return;
          }
        }
        throw std::invalid_argument("Criterion not found in performance table");
      }
    }
    throw std::invalid_argument("Name not found in performance table");
  } else if (mode_ == "crit") {
    for (std::vector<Perf> &p : pt_) {
      if (p[0].getCrit() == crit) {
        for (Perf &perf : p) {
          if (perf.getName() == name) {
            perf.setValue(value);
            return;
          }
        }
        throw std::invalid_argument("Name not found in performance table");
      }
    }
    throw std::invalid_argument("Criterion not found in performance table");
  } else {
    throw std::domain_error("Performance table mode corrupted.");
  }
}
