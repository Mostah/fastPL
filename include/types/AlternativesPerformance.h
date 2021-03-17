#ifndef AlternativesPerformance_H
#define AlternativesPerformance_H

#include "Category.h"
#include "Criteria.h"
#include "PerformanceTable.h"
#include <iostream>
#include <iterator>
#include <ostream>
#include <string.h>
#include <unordered_map>
#include <vector>

extern Category default_cat;
extern std::unordered_map<std::string, Category> default_map;

class AlternativesPerformance : public PerformanceTable {
public:
  /**
   * AlternativesPerformance standard constructor (PerformanceTable surcharged)
   *
   * @param perf_vect Vector of performance
   * @param alt_assignment Map of alternative to the assigned category.
   * Default alternatives assigned to empty categories, but good practice would
   * be to create the AlternativePerformance object with an
   * AlternativeAssignment map, otherwise we should use PerformanceTable instead
   */
  AlternativesPerformance(
      std::vector<std::vector<Perf>> &perf_vect,
      std::unordered_map<std::string, Category> &alt_assignment = default_map);

  /**
   * AlternativesPerformance constructor without perf values but set of
   * criteria to evaluate performance over (PerformanceTable surcharged)
   *
   * @param nb_of_perfs Number of performance
   * @param crits Criteria to evaluate performance over
   * @param prefix Prefix to use for the name of each Performance created.
   * Default = "alt"
   * @param alt_assignment Map of alternative to the assigned category.
   * Default alternatives assigned to empty categories, but good practice would
   * be to create the AlternativePerformance object with an
   * AlternativeAssignment map, otherwise we should use PerformanceTable instead
   * */
  AlternativesPerformance(
      int nb_of_perfs, Criteria &crits, std::string prefix = "alt",
      std::unordered_map<std::string, Category> &alt_assignment = default_map);

  /**
   * AlternativesPerformance constructor using an existing performance table
   *
   * @param perf_table Performance table to copy
   * @param alt_assignment Map of alternative assignements to categories
   * Default alternatives assigned to empty categories, but good practice would
   * be to create the AlternativePerformance object with an
   * AlternativeAssignment map, otherwise we should use PerformanceTable instead
   */
  AlternativesPerformance(
      const PerformanceTable &perf_table,
      std::unordered_map<std::string, Category> &alt_assignment = default_map);

  /**
   * AlternativesPerformance constructor by copy
   *
   * @param perfs Based performances to copy
   */
  AlternativesPerformance(const AlternativesPerformance &alt);

  ~AlternativesPerformance();

  friend std::ostream &operator<<(std::ostream &out,
                                  const AlternativesPerformance &alt);

  /**
   * getAlternativesPerformanceMap getter of the alternatives assignments
   *
   * @return alt_assignment_
   */
  std::unordered_map<std::string, Category> getAlternativesAssignments() const;

  /**
   * setAlternativesPerformanceMap getter of the alternatives assignments
   *
   * @param alt_assignment assignment map
   */
  void setAlternativesAssignments(
      std::unordered_map<std::string, Category> &alt_assignment);

  /**
   * getAlternative getter of the assignment of one specified alternative
   *
   * @param altName name of the alternative of which the assignment is requested
   * @return assignment of the alternative
   */
  Category getAlternativeAssignment(std::string altName) const;

  /**
   * setAlternative getter of the assignment of one specified alternative
   *
   * @param altName name of the alternative the category will be assigned to
   * @param cat category to assign
   */
  void setAlternativeAssignment(std::string altName, Category &cat);

  /**
   * getNumberCats compute the number of unique category in the dataset
   *
   * @return n_cats
   */
  int getNumberCats();

private:
  // Hashmap: key = Alternative Name, value = Category
  std::unordered_map<std::string, Category> alt_assignment_;
};

#endif