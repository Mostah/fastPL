#include <iostream>
#include <string>

#include "AtomicMCDAObject.h"

class CriterionValue : public AtomicMCDAObject {
public:
  /**
   * CriterionValue standard constructor
   *
   * @param id Id name of the criterion
   * @param weight Weight of the criterion
   */
  CriterionValue(std::string id, float weight);

  /**
   * CriterionValue Constructor without weight
   *
   * @param id Id name of the criterion
   */
  CriterionValue(std::string id);

  /**
   * CriterionValue constructor by copy
   *
   * @param cv Based criterion value to copy
   */
  CriterionValue(const CriterionValue &cv);

  ~CriterionValue();

  friend std::ostream &operator<<(std::ostream &out, const CriterionValue &cv);

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
  float weight_;
};
