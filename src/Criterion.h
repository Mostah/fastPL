#ifndef CRITERION_H
#define CRITERION_H

#include <iostream>
#include <string>
#include <vector>

#include "AtomicMCDAObject.h"

class Criterion : public AtomicMCDAObject {
public:
  /**
   * Criterion standard constructor
   *
   * @param id Identifier of the criterion
   * @param direction Equal to -1 if criterion is to minimize, 1 if the
   * criterion is to maximize
   * @param weight Weight of the criterion
   */
  Criterion(std::string id, int direction = 1, float weight = 0.);

  /**
   * Criterion constructor by copy
   *
   * @param crit Based criterion to copy
   */
  Criterion(const Criterion &crit);

  ~Criterion();
  friend std::ostream &operator<<(std::ostream &out, const Criterion &crit);

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
  int direction_;
  float weight_;
};

#endif