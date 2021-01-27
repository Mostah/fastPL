#include "../../include/types/AlternativesPerformance.h"
#include "../../include/utils.h"
#include <typeinfo>

Category default_cat;
std::unordered_map<std::string, Category> default_map;

AlternativesPerformance::AlternativesPerformance(
    std::vector<Performance> &perf_vect,
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
  out << "AlternativesPerformance( PerformanceTable[ " << std::endl;
  for (std::vector<Perf> p : alt.pt_) {
    out << "Performance: ";
    for (Perf perf : p) {
      out << perf << " ";
    }
    out << "| " << std::endl;
  }
  out << "], AlternativesAssignment{ ";

  out << alt.alt_assignment_;
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