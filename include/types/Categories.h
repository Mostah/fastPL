#ifndef CATEGORIES_H
#define CATEGORIES_H

/**
 * @file Categories.h
 * @brief Datastructure representing a set of category.
 *
 */

#include "Category.h"
#include <iostream>
#include <vector>

/** @class Categories Categories.h
 *  @brief Set of Category datastructure
 *
 * The Categories datastructure represents a full set of category, well
 * formatted to be used in the problem. It is implemented with a c++ Vector.
 *
 * It is used to represents all the category defined in a dataset. Each
 * category must be different for all the others, and more specifically, the
 * rank must be correctly defined such that we can decide which category is
 * better when comparing two single categories. Futhermore, the categories must
 * be ranked such that when itterating over the categories, we go from the best
 * one (higest rank) to the lowest one. Therefore, categories[0] always
 * represents the best category of the set.
 */
class Categories {
public:
  /**
   * Categories standard constructor 1
   *
   * @param number_of_categories number of categories wanted
   * @param prefix prefix given to each category ids.
   */
  Categories(int number_of_categories = 2, std::string prefix = "cat");

  /**
   * Categories standard constructor 2
   *
   * @param vect_category_ids vector of category ids
   *
   */
  Categories(std::vector<std::string> vect_category_ids);

  /**
   *Categories constructor by copy
   *
   * @param number_of_categories number of categories wanted
   */
  Categories(const Categories &categories);
  ~Categories();

  /**
   * Getter all the ranks from Categories object
   *
   * @return vector of category ranks
   */
  std::vector<int> getRankCategories();

  /**
   * Setter all the ranks from Categories object
   *
   * @param set_ranks vector of new category ranks
   *
   */
  void setRankCategories(std::vector<int> &set_ranks);

  /**
   * Setter all the ranks from Categories object
   * Ranks start at 0 and end until length of Categories vector.
   *
   *
   */
  void setRankCategories();

  /**
   * Getter all the ids from Categories object
   *
   * @return vector of Category ids
   */
  std::vector<std::string> getIdCategories();

  /**
   * Set a new values of Category ids from Categories object
   *
   * @param prefix prefix given to Category ids.
   *
   */
  void setIdCategories(std::string prefix);

  /**
   * Set a new values of Category ids from Categories object
   *
   * @param set_category_ids vector of new Category id for Categories object
   *
   */
  void setIdCategories(std::vector<std::string> &set_category_ids);

  /** getCategoryOfRank
   * get the category corresponding to the requested rank in the vector.
   *
   * @param rank rank of the category to return
   *
   */
  Category getCategoryOfRank(int rank);

  /**
   * Set a new values of Category ids from Categories object
   *
   * @param set_category_ids vector of new Category id for Categories object
   *
   */
  int getNumberCategories();

  /**
   * Overloading << operator for Categories class
   *
   * @param out ostream
   * @param categories Categories object
   *
   */
  friend std::ostream &operator<<(std::ostream &out,
                                  const Categories &categories);

  /**
   * Overloading [] operator for Categories
   *
   * @param index index of the object we want
   *
   *@return Category object at index position of Categories object
   */
  Category operator[](int index);
  Category operator[](int index) const;

private:
  std::vector<Category> categories_vector_;
};

/**
 * Overloading << operator for std::vector object
 *
 * @param out ostream
 * @param vec std::vector object
 *
 */
template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec);

#endif