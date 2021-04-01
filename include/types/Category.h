#ifndef CATEGORY_H
#define CATEGORY_H

/**
 * @file Category.h
 * @brief Datastructure representing a category.
 *
 */

#include <iostream>

/** @class Category Category.h
 * @brief Category datastructure
 *
 * The Category datastructure is represented by its name and its rank. The rank
 * represents the order to assume when comparing two categories. The rank 0 is
 * by convention the worst category.
 */

class Category {
public:
  /**
   * Category standard constructor
   *
   * @param category_id Category id
   * @param cat_rank rank of Category
   */
  Category(std::string category_id = "", int cat_rank = -1);

  /**
   * Category constructor by copy
   *
   * @param category base Category to copy
   */
  Category(const Category &category);

  ~Category(){};

  /**
   * Overloading << operator for Category class
   *
   * @param out ostream
   * @param cat Category object
   *
   */
  friend std::ostream &operator<<(std::ostream &out, const Category &category);

  /**
   * Overloading == operator for Category class
   *
   * @param cat1 Category object 1
   * @param cat2 Category object 2
   *
   */
  friend bool operator==(const Category &cat1, const Category &cat2);

  std::string category_id_;
  int rank_;
};

#endif