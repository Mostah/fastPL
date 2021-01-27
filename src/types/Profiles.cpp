#include "../../include/types/Profiles.h"
#include "../../include/types/Perf.h"
#include "../../include/types/Performance.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

Profiles::Profiles(std::vector<Performance> &perf_vect)
    : PerformanceTable(perf_vect) {
  mode_ = "crit";
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
  mode_ = "crit";
  sorted_ = 1;
}

Profiles::Profiles(const Profiles &profiles) : PerformanceTable(profiles) {
  mode_ = "crit";
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

void Profiles::display() {
  // By default of Profile is in "crit" mode by for displaying purpose we will
  // print it in "alt" mode since it it easier to read Profiles that way
  int nbFictAlt = pt_.size();
  int nbCriteria = pt_[0].size();

  int lengthLongestCriteriaIds = pt_.back().back().getCrit().size();
  int lengthLongestAlternativeIds = pt_[0].back().getName().size();
  float PerformanceValue = pt_[0].front().getValue();
  std::stringstream ss1;
  ss1 << PerformanceValue;
  std::string str1 = ss1.str();
  int lengthPerformanceValue = str1.size();
  int tmp = std::max(lengthLongestCriteriaIds, lengthPerformanceValue);
  if (tmp == lengthLongestCriteriaIds) {
    std::cout << std::string(lengthLongestAlternativeIds + 1, ' ');
    for (int i = 0; i < nbCriteria; i++) {
      std::string crit = pt_[i][0].getCrit();
      std::cout << crit;
      std::cout << std::string(1, ' ');
    }
    std::cout << std::endl;

    for (int i = 0; i < nbFictAlt; i++) {
      std::cout << pt_[i][0].getName() << std::string(1, ' ');
      for (int j = 0; j < nbCriteria; j++) {
        float value = pt_[i][j].getValue();
        std::stringstream ss1;
        ss1 << value;
        std::string str1 = ss1.str();
        std::cout << value;
        std::cout << std::string(
            lengthLongestCriteriaIds - ss1.str().size() + 1, ' ');
      }
      std::cout << std::endl;
    }
  } else {
    std::cout << std::string(lengthLongestAlternativeIds + 1, ' ');

    for (int i = 0; i < nbCriteria; i++) {
      std::string crit = pt_[i][0].getCrit();
      std::cout << crit;
      std::cout << std::string(lengthPerformanceValue - crit.size() + 1, ' ');
    }
    std::cout << std::endl;

    for (int i = 0; i < nbFictAlt; i++) {
      std::cout << pt_[i][0].getName();
      std::cout << std::string(1, ' ');
      for (int j = 0; j < nbCriteria; j++) {
        std::cout << pt_[i][j].getValue();
        std::cout << std::string(1, ' ');
      }
      std::cout << std::endl;
    }
  }
}

bool Profiles::isProfileOrdered() {
  for (int crit = 0; crit < pt_.size() - 1; crit++) {
    for (int catLimit = 0; catLimit < pt_[crit].size(); catLimit++) {
      if (pt_[crit][catLimit].getValue() < pt_[crit + 1][catLimit].getValue()) {
        return false;
      }
    }
  }
  return true;
}
