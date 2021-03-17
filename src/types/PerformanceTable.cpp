#include "../../include/types/PerformanceTable.h"
#include "../../include/types/Perf.h"
#include "../../include/utils.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <chrono>
#include <thread>

PerformanceTable::PerformanceTable(std::vector<std::vector<Perf>> &perf_vect,
                                   std::string mode) {
  if (perf_vect.size() == 0) {
    throw std::invalid_argument("The vector must contain performances.");
  }
  if (mode != "alt" && mode != "crit") {
    throw std::invalid_argument("Mode must be alt or crit.");
  }
  mode_ = mode;
  if (mode == "alt") {
    std::vector<std::string> perf_id_vect;
    std::vector<std::string> crit_vect = getCriterionIds(perf_vect[0]);

    for (std::vector<Perf> p : perf_vect) {
      // ensure there is no performance with dupplicated name

      if (std::find(perf_id_vect.begin(), perf_id_vect.end(), p[0].getName()) !=
          perf_id_vect.end()) {
        throw std::invalid_argument(
            "Each performance must have different ids.");
      }
      perf_id_vect.push_back(p[0].getName());

      // ensure all the performance are based on the same set of criterion
      if (getCriterionIds(p) != crit_vect) {
        throw std::invalid_argument(
            "Each performance must be based on the same "
            "set of criterion, in the same order.");
      }
      pt_.push_back(p);
    }
  } else {
    std::vector<std::string> crit_id_vect;
    std::vector<std::string> altIdVec = getNameIds(perf_vect[0]);

    for (std::vector<Perf> p : perf_vect) {
      // ensure there is no criteria id with duplicated name

      if (std::find(crit_id_vect.begin(), crit_id_vect.end(), p[0].getCrit()) !=
          crit_id_vect.end()) {
        throw std::invalid_argument("Each row must have different criterias.");
      }
      crit_id_vect.push_back(p[0].getCrit());

      // ensure all the performance are based on the same set of criterion
      if (getNameIds(p) != altIdVec) {
        throw std::invalid_argument(
            "Each criteria row must be based on the same "
            "set of alternative ids, in the same order.");
      }
      pt_.push_back(p);
    }
  }
}

PerformanceTable::PerformanceTable(int nb_of_perfs, Criteria &crits,
                                   std::string prefix) {
  for (int i = 0; i < nb_of_perfs; i++) {
    std::vector<Perf> alt;
    for (Criterion criterion : crits.getCriterionVect()) {
      alt.push_back(Perf(prefix + std::to_string(i), criterion.getId(), 0));
    }
    pt_.push_back(alt);
  }
}

PerformanceTable::PerformanceTable(const PerformanceTable &perfs) {
  // deep copy
  for (std::vector<Perf> p : perfs.pt_) {
    std::vector<Perf> perf_vect;
    for (Perf perf : p) {
      perf_vect.push_back(Perf(perf));
    }
    pt_.push_back(perf_vect);
  }
  mode_ = perfs.mode_;
  sorted_ = perfs.sorted_;
}

PerformanceTable::~PerformanceTable() {}

std::ostream &operator<<(std::ostream &out, const PerformanceTable &perfs) {
  out << "PerformanceTable[ ";
  for (std::vector<Perf> p : perfs.pt_) {
    out << "Performance: ";
    for (Perf perf : p) {
      out << perf << " ";
    }
    out << "| ";
  }
  out << "]";
  return out;
}

std::vector<Perf> PerformanceTable::operator[](std::string name) const {
  // supposing the pt is consistent:
  // if in mode alt, each row contains 1 and only 1 (alternative or profile)
  // if in mode crit, each row contains 1 and only 1 criterion
  if (mode_ == "alt") {
    for (std::vector<Perf> p : pt_) {
      if (p[0].getName() == name) {
        return p;
      }
    }
    throw std::invalid_argument("Row not found in performance table");
  } else if (mode_ == "crit") {
    for (std::vector<Perf> p : pt_) {
      if (p[0].getCrit() == name) {
        return p;
      }
    }
    throw std::invalid_argument("Row not found in performance table");
  } else {
    throw std::domain_error("Performance table mode corrupted.");
  }
}

void PerformanceTable::generateRandomPerfValues(unsigned long int seed,
                                                int lower_bound,
                                                int upper_bound) {
  if (lower_bound > upper_bound) {
    throw std::invalid_argument(
        "Lower bound must be lower than the upper bound.");
  }
  std::random_device rd;
  for (std::vector<Perf> &pv : pt_) {
    for (Perf &p : pv) {
      p.setValue(getRandomUniformFloat(rd(), lower_bound, upper_bound));
    }
  }
  sorted_ = false;
}

Perf PerformanceTable::getPerf(std::string name, std::string crit) const {
  // supposing the pt is consistent:
  // if in mode alt, each row contains 1 and only 1 (alternative or profile)
  // if in mode crit, each row contains 1 and only 1 criterion
  if (mode_ == "alt") {
    for (std::vector<Perf> p : pt_) {
      if (p[0].getName() == name) {
        for (Perf perf : p) {
          if (perf.getCrit() == crit) {
            return perf;
          }
        }
        throw std::invalid_argument("Criterion not found in performance table");
      }
    }
    throw std::invalid_argument("Name not found in performance table");
  }
  if (mode_ == "crit") {
    for (std::vector<Perf> p : pt_) {
      if (p[0].getCrit() == crit) {
        for (Perf perf : p) {
          if (perf.getName() == name) {
            return perf;
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

std::vector<std::vector<Perf>> PerformanceTable::getPerformanceTable() const {
  return pt_;
}

std::string PerformanceTable::getMode() const { return mode_; }

bool PerformanceTable::isSorted() const { return sorted_; }

void PerformanceTable::changeMode(std::string mode) {
  // If nothing to change, return early
  if (mode == mode_) {
    return;
  }
  if (mode != "alt" && mode != "crit") {
    throw std::invalid_argument("Mode must be alt or crit.");
  }
  // this operation cannot be done in place, should not be an issue as we
  // should not call this method often.
  std::vector<std::vector<Perf>> new_pt;
  std::map<std::string, int> index;
  if (mode == "crit") {
    for (std::vector<Perf> pv : pt_) {
      for (Perf p : pv) {
        if (index.count(p.getCrit()) > 0) {
          // if criterion already seen, add Perf to the right row
          new_pt[index[p.getCrit()]].push_back(p);
        } else {
          // create a new row for this criterion and add Perf
          std::vector<Perf> v = {p};
          new_pt.push_back(v);
          index[p.getCrit()] = new_pt.size() - 1; // index of crit in the new_pt
        }
      }
    }
  } else {
    for (std::vector<Perf> pv : pt_) {
      for (Perf p : pv) {
        if (index.count(p.getName()) > 0) {
          // if alternative (or profile) already seen, add Perf to the right
          // row
          new_pt[index[p.getName()]].push_back(p);
        } else {
          // if alternative (or profile) already seen, add Perf to the right
          // row
          std::vector<Perf> v = {p};
          new_pt.push_back(v);
          index[p.getName()] = new_pt.size() - 1; // index of crit in the new_pt
        }
      }
    }
  }
  pt_ = new_pt;
  mode_ = mode;
  sorted_ = false;
}

// OPTI could be optimized by using other sort methods if necessary
void PerformanceTable::sort(std::string mode) {
  // ensure the right state
  this->changeMode(mode);

  for (std::vector<Perf> &pv : pt_) {
    std::sort(pv.begin(), pv.end(), [](const Perf &a, const Perf &b) {
      return a.getValue() < b.getValue();
    });
  }
  sorted_ = true;
}

std::vector<Perf> PerformanceTable::getAltBetween(std::string critId, float inf,
                                                  float sup) {
  if (mode_ != "crit") {
    throw std::domain_error("Mode must be crit but is currently set to " +
                            mode_ + ".");
  }
  if (inf >= sup) {
    throw std::invalid_argument("Sup must be greater (>) than inf");
  }
  if (!sorted_) {
    throw std::domain_error("The performance table must be sorted.");
  }

  std::vector<Perf> pv = this->operator[](critId);
  auto lower_b = std::lower_bound(
      pv.begin(), pv.end(), inf,
      [](const Perf &a, const float b) { return a.getValue() < b; });
  // find the first index that have a value above sup
  auto upper_b = std::upper_bound(
      pv.begin(), pv.end(), sup,
      [](const float a, const Perf &b) { return a < b.getValue(); });

  if (lower_b != pv.end() && upper_b != pv.end()) {
    return subVector(pv, std::distance(pv.begin(), lower_b),
                     std::distance(pv.begin(), upper_b) - 1);
  } else if (upper_b == pv.end()) {
    return subVector(pv, std::distance(pv.begin(), lower_b), pv.size() - 1);
  } else {
    // return empty perf vect
    std::vector<Perf> v;
    return v;
  }
}

std::vector<Perf> PerformanceTable::getBestPerfByCrit(Criteria &crits) {
  // ensure the right state
  this->changeMode("crit");
  std::vector<Perf> best_pv;
  if (sorted_) {
    for (std::vector<Perf> pv : pt_) {
      if (crits[pv[0].getCrit()].getDirection() == 1) {
        // sorted in increasing way
        best_pv.push_back(Perf(pv[pv.size() - 1]));
      } else {
        best_pv.push_back(Perf(pv[0]));
      }
    }
  } else {
    for (std::vector<Perf> pv : pt_) {
      Perf bp = pv[0];
      for (Perf p : pv) {
        if (crits[pv[0].getCrit()].getDirection() == 1) {
          if (p.getValue() > bp.getValue()) {
            bp = p;
          }
        } else {
          if (p.getValue() < bp.getValue()) {
            bp = p;
          }
        }
      }
      best_pv.push_back(Perf(bp));
    }
  }
  return best_pv;
}

std::vector<Perf> PerformanceTable::getWorstPerfByCrit(Criteria &crits) {
  // ensure the right state
  this->changeMode("crit");
  std::vector<Perf> worst_pv;
  if (sorted_) {
    for (std::vector<Perf> pv : pt_) {
      if (crits[pv[0].getCrit()].getDirection() == -1) {
        // sorted in increasing way
        worst_pv.push_back(Perf(pv[pv.size() - 1]));
      } else {
        worst_pv.push_back(Perf(pv[0]));
      }
    }
  } else {
    for (std::vector<Perf> pv : pt_) {
      Perf bp = pv[0];
      for (Perf p : pv) {
        if (crits[pv[0].getCrit()].getDirection() == -1) {
          if (p.getValue() > bp.getValue()) {
            bp = p;
          }
        } else {
          if (p.getValue() < bp.getValue()) {
            bp = p;
          }
        }
      }
      worst_pv.push_back(Perf(bp));
    }
  }
  return worst_pv;
}

bool PerformanceTable::isAltInTable(std::string altName) {
  for (std::vector<Perf> p : pt_) {
    if (p[0].getName() == altName) {
      return (true);
    }
  }
  return (false);
}

int PerformanceTable::getNumberCrit() {
  if (mode_ == "alt") {
    return pt_[0].size();
  }
  return pt_.size();
}

int PerformanceTable::getNumberAlt() {
  if (mode_ == "crit") {
    return pt_[0].size();
  }
  return pt_.size();
}

void PerformanceTable::display() {
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
      std::string crit = pt_[0][i].getCrit();
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
      std::string crit = pt_[0][i].getCrit();
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

bool PerformanceTable::operator==(const PerformanceTable &pt) const {

  if (this->pt_.size() != pt.getPerformanceTable().size() ||
      this->pt_[0].size() != pt.getPerformanceTable()[0].size()) {
    return 0;
  }

  int nbPerfs = this->pt_.size();
  int nbCrits = this->pt_[0].size();
  for (int i = 0; i < nbPerfs; i++) {
    for (int j = 0; j < nbCrits; j++) {
      if (!(this->pt_[i][j] == pt.getPerformanceTable()[i][j])) {
        return false;
      }
    }
  }
  return true;
}
