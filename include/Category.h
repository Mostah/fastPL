#ifndef CATEGORY_H
#define CATEGORY_H

#include "AtomicMCDAObject.h"
#include <iostream>

class Category : public AtomicMCDAObject {
public:
  /**
   * Category standard constructor
   *
   * @param category_id Category id
   * @param cat_rank rank of Category
   */
  Category(std::string category_id, int cat_rank);

  /**
   * Category constructor by copy
   *
   * @param category base Category to copy
   */
  Category(const Category &category);

  ~Category(){};

  /**
   * Getter of category_id class attribute
   *
   * @return Category id
   */
  std::string getCategoryId() const;

  /**
   * Set a new value of category_id class attribute
   *
   * @param set_cat_id
   *
   */
  void setCategoryId(std::string set_cat_id);

  /**
   * Getter of rank class attribute
   * @return Category rank
   */
  int getCategoryRank() const;

  /**
   * Set a new value of rank class attribute
   *
   * @param set_rank
   *
   */
  void setCategoryRank(int set_rank);

  /**
   * Overloading << operator for Category class
   *
   * @param out ostream
   * @param cat Category object
   *
   */
  friend std::ostream &operator<<(std::ostream &out, const Category &category);

private:
  std::string category_id_;
  int rank_;
};

#endif