#include "Categories.h"
#include <iostream>
#include <string.h>

Categories::Categories(int number_of_categories, std::string prefix) {
  for (int i = 0; i < number_of_categories; i++) {
    std::string id = prefix + std::to_string(i);
    Category tmp_cat = Category(id, i);
    categories_vector_[i] = tmp_cat;
  }
}

Categories::Categories(int number_of_categories) {
  for (int i = 0; i < number_of_categories; i++) {
    std::string id = "cat" + std::to_string(i);
    Category tmp_cat = Category(id, i);
    categories_vector_[i] = tmp_cat;
  }
}

Categories::Categories(const Categories &categories) {
  for (int i = 0; i < categories_vector_.size(); i++) {
    categories_vector_[i] = categories[i];
  }
}

Category Categories::operator[](int index) { return categories_vector_[index]; }

Category Categories::operator[](int index) const {
  return categories_vector_[index];
}

Categories::~Categories() {
  // https://stackoverflow.com/questions/993590/should-i-delete-vectorstring
  std::vector<Category>().swap(categories_vector_);
}

std::vector<int> Categories::getRankCategories() {
  std::vector<int> rank_categories;
  for (int i = 0; i < categories_vector_.size(); i++) {
    rank_categories[i] = categories_vector_[i].getCategoryRank();
  }
  return rank_categories;
}

void Categories::setRankCategories(std::vector<int> &set_ranks) {
  if (set_ranks.size() != set_ranks.size()) {
    throw std::invalid_argument(
        "RankCategories setter object is not the same size as Categories.");
  }
  for (int i = 0; i < set_ranks.size(); i++) {
    categories_vector_[i].setCategoryRank(set_ranks[i]);
  }
}

void Categories::setRankCategories() {
  for (int i = 0; i < categories_vector_.size(); i++) {
    categories_vector_[i].setCategoryRank(i);
  }
}

std::vector<std::string> Categories::getIdCategories() {
  std::vector<std::string> categories_ids;
  for (int i = 0; i < categories_vector_.size(); i++) {
    categories_ids[i] = categories_vector_[i].getCategoryId();
  }
  return categories_ids;
}

void Categories::setIdCategories(std::string prefix) {
  for (int i = 0; i < categories_vector_.size(); i++) {
    categories_vector_[i].setCategoryId(prefix + std::to_string(i));
  }
}

void Categories::setIdCategories(std::vector<std::string> &set_category_ids) {
  if (set_category_ids.size() != categories_vector_.size()) {
    throw std::invalid_argument(
        "IdCategories setter object is not the same size as Categories.");
  }
  for (int i = 0; i < set_category_ids.size(); i++) {
    categories_vector_[i].setCategoryId(set_category_ids[i]);
  }
}

std::ostream &operator<<(std::ostream &out, const Categories &cats) {
  /**
   * Overloading << operator for Categories class
   *
   * @param cats
   *
   */
  out << "Categories( ";
  for (int i = 0; i < cats.categories_vector_.size(); i++) {
    out << "" << cats[i] << ",";
  }
  out << " )";
  return out;
}

std::ostream &operator<<(std::ostream &out, const std::vector<int> &vec) {
  /**
   * Overloading << operator for Categories class
   *
   * @param cats
   *
   */
  out << '[';
  for (std::vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i) {
    out << *i << ' ';
  }
  out << ']';
  return out;
}

std::ostream &operator<<(std::ostream &out,
                         const std::vector<std::string> &vec) {
  /**
   * Overloading << operator for Categories class
   *
   * @param vec
   *
   */
  out << '[';
  for (std::vector<std::string>::const_iterator i = vec.begin(); i != vec.end();
       ++i) {
    out << *i << ' ';
  }
  out << ']';
  return out;
}