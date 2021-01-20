#include "../../include/types/Profiles.h"
#include "../../include/types/Perf.h"
#include "../../include/types/Performance.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

Profiles::Profiles(std::vector<Performance> &perf_vect, std::string mode)
    : PerformanceTable(perf_vect) {
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
  mode_ = mode;
  this->generateRandomPerfValues();
}

Profiles::Profiles(const Profiles &profiles) : PerformanceTable(profiles) {}

Profiles Profiles::changeMode() {
  std::vector<Performance> perf_vec;
  std::string mode;
  if (mode_ == "crit") {
    mode = "alt";
    int nbCriteria = pt_.size();
    int nbFictAlt = pt_[0].size();
    // the number of column of pt_ is going to be the number of fictive
    // alternative in the new pt
    for (int i = 0; i < nbFictAlt; i++) {
      std::vector<Perf> perf;
      for (int j = 0; j < nbCriteria; j++) {
        perf.push_back(Perf("alt" + std::to_string(i),
                            "crit" + std::to_string(j), pt_[j][i].getValue()));
      }
      perf_vec.push_back(Performance(perf));
    }
  } else {
    mode = "crit";
    int nbFictAlt = pt_.size();
    int nbCriteria = pt_[0].size();
    // the number of column of pt_ is going to be the number of fictive
    // alternative in the new pt
    for (int i = 0; i < nbCriteria; i++) {
      std::vector<Perf> perf;
      for (int j = 0; j < nbFictAlt; j++) {
        perf.push_back(Perf("crit" + std::to_string(i),
                            "cat" + std::to_string(j), pt_[j][i].getValue()));
      }
      perf_vec.push_back(Performance(perf));
    }
  }
  return Profiles(perf_vec, mode);
}

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

void Profiles::display() {
  Performance tmpPerf = Performance(pt_[0]);
  std::vector<std::string> criteriaIds = tmpPerf.getCriterionIds();
  int lengthCriteriaIds = criteriaIds[0].size();
  int lengthAlternativeIds = tmpPerf.getId().size();
  std::cout << std::string(lengthAlternativeIds + 2, ' ');
  for (std::string crit_id : criteriaIds) {
    std::cout << crit_id << std::string(std::max(lengthCriteriaIds, 4), ' ');
  }
  std::cout << std::endl;
  for (Performance p : pt_) {
    std::cout << p.getId() << "  ";
    for (Perf perf : p.getPerf()) {
      std::cout << perf.getValue() << std::string(lengthCriteriaIds + 2, ' ');
    }
    std::cout << std::endl;
  }
}

bool Profiles::isProfileOrdered() {
  if (mode_ == "alt") {
    for (int i = 0; i < pt_.size() - 1; i++) {
      for (int j = 0; j < pt_[i].size(); j++) {
        if (pt_[i][j].getValue() < pt_[i + 1][j].getValue()) {
          return false;
        }
      }
    }
    return true;
  } else if (mode_ == "crit") {
    for (int crit = 0; crit < pt_.size() - 1; crit++) {
      for (int catLimit = 0; catLimit < pt_[crit].size(); catLimit++) {
        if (pt_[crit][catLimit].getValue() <
            pt_[crit + 1][catLimit].getValue()) {
          return false;
        }
      }
    }
    return true;
  }
}