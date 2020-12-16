#include "PerformanceTable.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

PerformanceTable::PerformanceTable(std::vector<Performance> &perf_vect) {
  if (perf_vect.size() == 0) {
    throw std::invalid_argument("The vector must contain performances.");
  }
  std::vector<std::string> perf_id_vect;
  std::vector<std::string> crit_vect = perf_vect[0].getCriteria();

  for (int i = 0; i < perf_vect.size(); i++) {
    // ensure there is no performance with dupplicated name
    if (std::find(perf_id_vect.begin(), perf_id_vect.end(),
                  perf_vect[i].getId()) != perf_id_vect.end()) {
      throw std::invalid_argument("Each performance must have different ids.");
    }
    perf_id_vect.push_back(perf_vect[i].getId());

    // ensure all the performance are based on the same set of criterion
    if (perf_vect[i].getCriteria() != crit_vect) {
      throw std::invalid_argument("Each performance must be based on the same "
                                  "set of criterion, in the same order.");
    }
    pt_.push_back(Performance(perf_vect[i]).getPerf());
  }
  mode_ = "alt";
}

PerformanceTable::PerformanceTable(std::string prefix, int nb_of_perfs,
                                   Criteria crits) {
  for (int i = 0; i < nb_of_perfs; i++) {
    pt_.push_back(Performance(prefix + std::to_string(i), crits).getPerf());
  }
  mode_ = "alt";
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
  mode_ = "alt";
}

PerformanceTable::~PerformanceTable() {}

std::ostream &operator<<(std::ostream &out, const PerformanceTable &perfs) {
  out << "PerformanceTable(";
  for (std::vector<Perf> p : perfs.pt_) {
    out << "Performance(";
    for (Perf perf : p) {
      out << perf << ", ";
    }
    out << "), ";
  }
  out << ")";
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

void PerformanceTable::changeMode(std::string mode) {
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
}

// OPTI could be optimized by using other sort methods if necessary
void PerformanceTable::sort(std::string mode) {
  if (mode != "alt" && mode != "crit") {
    throw std::invalid_argument("Mode must be alt or crit.");
  }
  if (mode == "alt" && mode_ != "alt") {
    this->changeMode("alt");
  } else if (mode == "crit" && mode_ != "crit") {
    this->changeMode("crit");
  }

  for (std::vector<Perf> &pv : pt_) {
    std::sort(pv.begin(), pv.end(), [](const Perf &a, const Perf &b) {
      return a.getValue() < b.getValue();
    });
  }
}

/*
  for (std::vector<Perf> v : new_pt) {
    for (Perf e : v) {
      std::cout << e.getName() << " " << e.getCrit() << " ";
    }
    std::cout << std::endl;
  }
  */