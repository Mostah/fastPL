#include "Category.h"
#include <iostream>
#include <vector>

class Categories {
public:
  Categories(int number_of_categories, std::string prefix);
  Categories(int number_of_categories);
  Categories(const Categories &categories);
  ~Categories();

  std::vector<int> getRankCategories();
  void setRankCategories(std::vector<int> &set_ranks);
  void setRankCategories();

  std::vector<std::string> getIdCategories();
  void setIdCategories(std::string prefix);
  void setIdCategories(std::vector<std::string> &set_category_ids);

  friend std::ostream &operator<<(std::ostream &out,
                                  const Categories &category);

  friend std::ostream &operator<<(std::ostream &out,
                                  const std::vector<int> &vec);

  friend std::ostream &operator<<(std::ostream &out,
                                  const std::vector<std::string> &vec);

  Category operator[](int index);
  Category operator[](int index) const;

private:
  std::vector<Category> categories_vector_;
};