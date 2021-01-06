#include "AlternativePerformance.h"

AlternativePerformance::AlternativePerformance(
    std::vector<Performance> &perf_vect,
    std::map<std::string, std::string> alt_assignment)
    : PerformanceTable(perf_vect) {
  // Need to check if alt_assignment is legit
  alt_assignment_ = alt_assignment;
}

AlternativePerformance::AlternativePerformance(
    std::vector<Performance> &perf_vect)
    : PerformanceTable(perf_vect) {
  if (mode_ != "alt") {
    throw std::domain_error(
        "Performance table mode should be alt to assign default categories.");
  }
  for (std::vector<Perf> pv : pt_) {
    std::string altName = pv[0].getName();
    alt_assignment_[altName] = "unknown category";
  }
}

AlternativePerformance::AlternativePerformance(
    int nb_of_perfs, Criteria &crits,
    std::map<std::string, std::string> alt_assignment)
    : PerformanceTable(nb_of_perfs, crits) {
  // Need to check if alt_assignment is legit
  alt_assignment_ = alt_assignment;
}

AlternativePerformance::AlternativePerformance(int nb_of_perfs, Criteria &crits)
    : PerformanceTable(nb_of_perfs, crits) {
  if (mode_ != "alt") {
    throw std::domain_error(
        "Performance table mode should be alt to assign categories.");
  }
  for (std::vector<Perf> pv : pt_) {
    std::string altName = pv[0].getName();
    alt_assignment_[altName] = "unknown category";
  }
}

AlternativePerformance::AlternativePerformance(
    const PerformanceTable &perf_table,
    std::map<std::string, std::string> alt_assignment)
    : PerformanceTable(perf_table) {
  alt_assignment_ = alt_assignment;
}

AlternativePerformance::AlternativePerformance(
    const PerformanceTable &perf_table)
    : PerformanceTable(perf_table) {
  if (perf_table.getMode() != "alt") {
    throw std::domain_error(
        "Performance table mode should be alt to assign categories.");
  }
  for (std::vector<Perf> pv : perf_table.getPerformanceTable()) {
    std::string altName = pv[0].getName();
    alt_assignment_[altName] = "unknown category";
  }
}

AlternativePerformance::AlternativePerformance(
    const AlternativePerformance &alt)
    : PerformanceTable(alt) {
  // How to implement the copy for PerfTable?
}

std::map<std::string, std::string>
AlternativePerformance::getAlternativeAssignments() const {
  return alt_assignment_;
}

std::string
AlternativePerformance::getAlternativeAssignment(std::string altName) const {
  return alt_assignment_.find(altName)->second;
}