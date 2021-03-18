#ifndef PERF_H
#define PERF_H

/**
 * @file Perf.h
 * @brief Perf (single performance) data structure.
 *
 */

#include <iostream>
#include <string>

/** @class Perf Perf.h
 * @brief Perf (single performance) data structure.
 *
 * Perf object is the representation of a single performance of an alternative
 * on a criterion: a_i_j A Perf is thus identified by a name (name of the
 * profile or the alternative), the name of the Criterion on which the
 * performance was evaluated, and the value of the performance.
 *
 */
class Perf {
public:
  /**
   * Perf standard constructor
   *
   * @param name name of the performance (ex: name of profile or alternative)
   * @param criterion criterion id on which the performance in based on
   * @param value value of the performance
   */
  Perf(std::string name, std::string criterion, float value);

  /**
   * Perf standard constructor
   *
   * @param name name of the performance (ex: name of profile or alternative)
   * @param criterion criterion id on which the performance in based on
   */
  Perf(std::string name, std::string criterion);

  /**
   * Performance constructor by copy
   *
   * @param perf Based performance to copy
   */
  Perf(const Perf &perf);

  ~Perf();

  friend std::ostream &operator<<(std::ostream &out, const Perf &p);

  /**
   * getName getter of name parameter
   *
   * @return name
   */
  std::string getName() const;

  /**
   * setName setter of name parameter
   *
   * @param name
   */
  void setName(std::string name);

  /**
   * getValue getter of value parameter
   *
   * @return value
   */
  float getValue() const;

  /**
   * setValue setter of value parameter
   *
   * @param value
   */
  void setValue(float value);

  /**
   * getCrit getter of crit parameter
   *
   * @return crit
   */
  std::string getCrit() const;

  /**
   * setCrit setter of crit parameter
   *
   * @param crit
   */
  void setCrit(std::string crit);

  /**
   * Overload of == operator for Perf object
   *
   * @param perf2 Perf object to compare it with
   */
  bool operator==(const Perf &perf2) const;

private:
  std::string crit_;
  float value_;
  std::string name_;
};

#endif