#ifndef CRITERIA_H
#define CRITERIA_H

#include "Criterion.h"
#include <iostream>
#include <vector>

class Criteria {
public:
  /**
   * Criteria constructor with defined vector of criteria
   *
   * @param criterion_vect Vector of criteria
   */
  Criteria(std::vector<Criterion> &criterion_vect);

  /**
   * Criteria constructor without vector
   *
   * @param nb_of_criterion Number of criteria
   * @param prefix Prefix to identify criteria
   */
  Criteria(int nb_of_criterion, std::string prefix = "crit");

  /**
   * Criteria constructor by copy
   *
   * @param crits Based criteria to copy
   */
  Criteria(const Criteria &crits);

  ~Criteria();

  friend std::ostream &operator<<(std::ostream &out, const Criteria &crits);

  /**
   * setCriterionVect setter of criterion vector parameter
   *
   * @param criterion_vect_
   */
  void setCriterionVect(std::vector<Criterion> &criterion_vect_);

  /**
   * getCriterionVect getter of criterion vector parameter
   *
   * @return criterion_vect_
   */
  std::vector<Criterion> getCriterionVect() const;

  /**
   * getMinWeight return the min criterion weight of this Criteria structure
   *
   * @return min_weight
   */
  float getMinWeight();

  /**
   * getMaxWeight return the max criterion weight of this Criteria structure
   *
   * @return max_weight
   */
  float getMaxWeight();

  /**
   * getSumWeight return the sum of criterion weight of this Criteria structure
   *
   * @return sum_weight
   */
  float getSumWeight();

  /**
   * getWeights returns an vector of all the weight from the Criteria structure
   *
   * @return get_weights
   */
  std::vector<float> getWeights() const;

  /**
   * Set new weights in the Criteria object
   *
   */
  void setWeights(std::vector<float> newWeights);
  /**
   * normalizeWeights normalizes the weights of each criterion in the Criteria
   * object
   *
   */
  void normalizeWeights();

  /**
   * Generates random Criteria weight for each Criterion
   *
   */
  void generateRandomCriteriaWeights(bool changeSeed = 1);

  /**
   * Overloading [] dict operator for Performance
   *
   * @param criteria criterion name of the Perf we want
   *
   * @return Perf object with associated criterion
   */
  Criterion operator[](std::string name) const;

  /**
   * Overloading [] operator for Criteria
   *
   * @param index index of the object we want
   *
   * @return Criterion object at index position of Criteria object
   */
  Criterion operator[](int index);
  Criterion operator[](int index) const;

private:
  std::vector<Criterion> criterion_vect_;
};

#endif