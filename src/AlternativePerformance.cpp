#include "AlternativePerformance.h"

AlternativePerformance::AlternativePerformance(
    std::map<std::string, std::string> alt_assignment,
    std::vector<Performance> &perf_vect)
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
    std::map<std::string, std::string> alt_assignment, int nb_of_perfs,
    Criteria &crits, std::string prefix)
    : PerformanceTable(nb_of_perfs, crits, prefix) {
  // Need to check if alt_assignment is legit
  alt_assignment_ = alt_assignment;
}

AlternativePerformance::AlternativePerformance(int nb_of_perfs, Criteria &crits,
                                               std::string prefix)
    : PerformanceTable(nb_of_perfs, crits, prefix) {
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
    std::map<std::string, std::string> alt_assignment,
    const PerformanceTable &perf_table)
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

AlternativePerformance::~AlternativePerformance() {}

std::ostream &operator<<(std::ostream &out, const AlternativePerformance &alt) {
  out << "AlternativePerformance(PerformanceTable(";
  for (std::vector<Perf> p : alt.pt_) {
    out << "Performance(";
    for (Perf perf : p) {
      out << perf << ", ";
    }
    out << "), ";
  }
  out << "), AlternativeAssignment: {";

  auto it = alt.alt_assignment_.begin();
  while (it != alt.alt_assignment_.end()) {
    out << it->first << " => " << it->second;
    it++;
  }
  out << "}";
  return out;
}

std::map<std::string, std::string>
AlternativePerformance::getAlternativeAssignments() const {
  return alt_assignment_;
}

std::string
AlternativePerformance::getAlternativeAssignment(std::string altName) const {
  return alt_assignment_.find(altName)->second;
}