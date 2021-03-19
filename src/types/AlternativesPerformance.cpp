#include "../../include/types/AlternativesPerformance.h"
#include "../../include/types/Category.h"
#include "../../include/utils.h"
#include <typeinfo>

Category default_cat;
std::unordered_map<std::string, Category> default_map;

AlternativesPerformance::AlternativesPerformance(
    std::vector<std::vector<Perf>> &perf_vect,
    std::unordered_map<std::string, Category> &alt_assignment)
    : PerformanceTable(perf_vect) {
  if (mode_ != "alt") {
    throw std::domain_error(
        "Performance table mode should be alt to assign default categories.");
  }
  if (alt_assignment.empty()) {
    for (std::vector<Perf> pv : pt_) {
      std::string altName = pv[0].getName();
      alt_assignment_[altName] = default_cat;
    }
  } else {
    // Check if the alternatives are in the performance table
    for (std::pair<std::string, Category> element : alt_assignment) {
      if (!this->isAltInTable(element.first)) {
        throw std::invalid_argument("The alternatives in the map should be "
                                    "present in the performance table.");
      }
    }
    alt_assignment_ = alt_assignment;
  }
}

AlternativesPerformance::AlternativesPerformance(
    int nb_of_perfs, Criteria &crits, std::string prefix,
    std::unordered_map<std::string, Category> &alt_assignment)
    : PerformanceTable(nb_of_perfs, crits, prefix) {
  if (mode_ != "alt") {
    throw std::domain_error(
        "Performance table mode should be alt to assign default categories.");
  }
  if (alt_assignment.empty()) {
    for (std::vector<Perf> pv : pt_) {
      std::string altName = pv[0].getName();
      alt_assignment_[altName] = default_cat;
    }
  } else {
    // Check if the alternatives are in the performance table
    for (std::pair<std::string, Category> element : alt_assignment) {
      if (!this->isAltInTable(element.first)) {
        throw std::invalid_argument("The alternatives in the map should be "
                                    "present in the performance table.");
      }
    }
    alt_assignment_ = alt_assignment;
  }
}

AlternativesPerformance::AlternativesPerformance(
    const PerformanceTable &perf_table,
    std::unordered_map<std::string, Category> &alt_assignment)
    : PerformanceTable(perf_table) {
  if (alt_assignment.empty()) {
    if (mode_ != "alt") {
      throw std::domain_error("Performance table mode should be alt to "
                              "assign default categories.");
    }
    for (std::vector<Perf> pv : pt_) {
      std::string altName = pv[0].getName();
      alt_assignment_[altName] = default_cat;
    }
  } else {
    // Check if the alternatives are in the performance table
    for (std::pair<std::string, Category> element : alt_assignment) {
      if (!this->isAltInTable(element.first)) {
        throw std::invalid_argument(
            "The alternatives in the map should be present "
            "in the performance table.");
      }
    }
    alt_assignment_ = alt_assignment;
  }
}

AlternativesPerformance::AlternativesPerformance(
    const AlternativesPerformance &alt)
    : PerformanceTable(alt) {
  alt_assignment_ = alt.alt_assignment_;
}

AlternativesPerformance::~AlternativesPerformance() {}

std::ostream &operator<<(std::ostream &out,
                         const AlternativesPerformance &alt) {
  out << "AlternativesPerformance( PerformanceTable[ ";
  for (std::vector<Perf> p : alt.pt_) {
    out << "Performance: ";
    for (Perf perf : p) {
      out << perf << " ";
    }
    out << "| ";
  }
  out << "], AlternativesAssignment{ ";

  auto it = alt.alt_assignment_.begin();
  while (it != alt.alt_assignment_.end()) {
    out << it->first << "->" << it->second << " ";
    it++;
  }
  out << "}";
  return out;
}

std::unordered_map<std::string, Category>
AlternativesPerformance::getAlternativesAssignments() const {
  return alt_assignment_;
}

void AlternativesPerformance::setAlternativesAssignments(
    std::unordered_map<std::string, Category> &alt_assignment) {
  // Check if the alternatives are in the performance table
  for (std::pair<std::string, Category> element : alt_assignment) {
    if (!this->isAltInTable(element.first)) {
      throw std::invalid_argument(
          "The alternatives in the map should be present "
          "in the performance table.");
    }
  }
  alt_assignment_ = alt_assignment;
}

Category
AlternativesPerformance::getAlternativeAssignment(std::string altName) const {
  return alt_assignment_.find(altName)->second;
}

void AlternativesPerformance::setAlternativeAssignment(std::string altName,
                                                       Category &cat) {
  if (this->isAltInTable(altName)) {
    alt_assignment_[altName] = cat;
  } else {
    throw std::invalid_argument("The alternatives in the map should be present "
                                "in the performance table.");
  }
}

int AlternativesPerformance::getNumberCats() {
  std::vector<std::string> cat_vector;
  for (auto pair : alt_assignment_) {
    if (!std::count(cat_vector.begin(), cat_vector.end(),
                    pair.second.getCategoryId())) {
      cat_vector.push_back(pair.second.getCategoryId());
    }
  }
  return cat_vector.size();
}

std::pair<float, float> AlternativesPerformance::getBoundaries() {
  float min = pt_[0][0].getValue();
  float max = pt_[0][0].getValue();
  for (int i = 0; i < pt_.size(); i++) {
    for (int j = 0; j < pt_[i].size(); j++) {
      if (pt_[i][j].getValue() < min) {
        min = pt_[i][j].getValue();
      }
      if (pt_[i][j].getValue() > max) {
        max = pt_[i][j].getValue();
      }
    }
  }

  return std::pair<float, float>(min, max);
}