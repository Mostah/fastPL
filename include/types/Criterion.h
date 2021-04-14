#ifndef CRITERION_H
#define CRITERION_H

/**
 * @file Criterion.h
 * @brief Datastructure representing a criterion.
 *
 */

#include <iostream>
#include <string>
#include <vector>

/** @class Criterion Criterion.h
 * @brief Criterion datastructure
 *
 * The Criterion datastructure is represented by its name, its weight and its
 * direction, representing the minimization of maximization of this criterion.
 */

class Criterion {
public:
  /**
   * Criterion standard constructor
   *
   * @param id Identifier of the criterion
   * @param direction Equal to -1 if criterion is to minimize, 1 if the
   * criterion is to maximize
   * @param weight Weight of the criterion
   */
  Criterion(std::string id, int direction = 1, float weight = 0.0f);

  /**
   * Criterion constructor by copy
   *
   * @param crit Based criterion to copy
   */
  Criterion(const Criterion &crit);

  ~Criterion();

  friend std::ostream &operator<<(std::ostream &out, const Criterion &crit);

  /**
   * generateDirection generator of random direction
   * can also be used as reinitialisation for direction
   *
   * @param seed Optional: seed for random generation. Set to 0 by
   * default
   *
   */
  void generateDirection(unsigned long int seed = 0);

  /**
   * generateWeight generator of random weight
   * can also be used as reinitialisation for weight
   *
   * @param seed Optional: seed for random generation. Set to 0 by
   * default
   *
   */
  void generateWeight(unsigned long int seed = 0);

  // TODO Remove getter and setter
  // Performance wise after some profiling we found that getter and setter,
  // especially for elementary classes can loose a lot of time compared to
  // access directly the variable. Therefore in order to optimize the code, they
  // should be removed.

  /**
   * getId getter of id parameter
   *
   * @return id
   */
  std::string getId() const;

  /**
   * setId setter of id parameter
   *
   * @param id
   */
  void setId(std::string id);

  /**
   * getName getter of name parameter
   *
   * @return name
   */
  int getDirection() const;

  /**
   * setDirection setter of direction parameter
   *
   * @param direction
   */
  void setDirection(int direction);

  /**
   * getWeight getter of weight parameter
   *
   * @return weight
   */
  float getWeight() const;

  /**
   * setWeight setter of weight parameter
   *
   * @param weight
   */
  void setWeight(float weight);

private:
  std::string id_;
  // TODO remove direction_ attribute
  // Originally, the direction_ attribute has a signification in the thesis of
  // Sobrie, it informs if we should maximize or minimize the specific
  // criterion. In the current implementation, we did not implement this
  // feature, therefore this arguments is obesolete and should be removed during
  // a code clean.
  int direction_;
  float weight_;
};

#endif