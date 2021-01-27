#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

PerformanceTable::PerformanceTable(std::vector<Performance> &perf_vect) {
  if (perf_vect.size() == 0) {
    throw std::invalid_argument("The vector must contain performances.");
  }
  std::vector<std::string> perf_id_vect;
  std::vector<std::string> crit_vect = perf_vect[0].getCriterionIds();

  for (Performance p : perf_vect) {
    // ensure there is no performance with dupplicated name
    if (std::find(perf_id_vect.begin(), perf_id_vect.end(), p.getId()) !=
        perf_id_vect.end()) {
      throw std::invalid_argument("Each performance must have different ids.");
    }
    perf_id_vect.push_back(p.getId());

    // ensure all the performance are based on the same set of criterion
    if (p.getCriterionIds() != crit_vect) {
      throw std::invalid_argument("Each performance must be based on the same "
                                  "set of criterion, in the same order.");
    }
    pt_.push_back(Performance(p).getPerf());
  }
}

PerformanceTable::PerformanceTable(int nb_of_perfs, Criteria &crits,
                                   std::string prefix) {
  for (int i = 0; i < nb_of_perfs; i++) {
    pt_.push_back(Performance(crits, prefix + std::to_string(i)).getPerf());
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
  for (std::vector<Perf> &pv : pt_) {
    for (Perf &p : pv) {
      p.setValue(getRandomUniformFloat(seed, lower_bound, upper_bound));
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
  // this operation cannot be done in place, should not be an issue as we should
  // not call this method often.
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
          // if alternative (or profile) already seen, add Perf to the right row
          new_pt[index[p.getName()]].push_back(p);
        } else {
          // if alternative (or profile) already seen, add Perf to the right row
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