#include "../../include/types/Category.h"

Category::Category(std::string cat_id, int cat_rank) {
  category_id_ = cat_id;
  rank_ = cat_rank;
}

Category::Category(const Category &cat) {
  category_id_ = cat.category_id_;
  rank_ = cat.rank_;
}

std::ostream &operator<<(std::ostream &out, const Category &cat) {

  out << "Category(id : " << cat.category_id_ << ", rank : " << cat.rank_
      << ")";
  return out;
}

bool operator==(const Category &cat1, const Category &cat2) {
  return (cat1.rank_ == cat2.rank_ && cat1.category_id_ == cat2.category_id_);
}