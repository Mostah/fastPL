#include "Category.h"


Category::Category(std::string cat_id, int cat_rank){
    category_id_ = cat_id ;
    rank_= cat_rank;
}

Category::Category(const Category & cat) {
    /**
    Copy constructor
    */
    category_id_ = cat.getCategoryId();
    rank_ = cat.getCategoryRank();
}

std::string Category::getCategoryId() const{
    /**
    * Getter of category_id class attribute
    */
    return category_id_;
}

void Category::setCategoryId(std::string set_cat_id){
    /**
    * Set a new value of category_id class attribute
    *
    * @param set_cat_id 
    * 
    */
    category_id_ = set_cat_id;
}

int Category::getCategoryRank() const{
    /**
    * Getter of rank class attribute
    */
    return rank_; 
}

void Category::setCategoryRank(int set_rank){
    /**
    * Set a new value of rank class attribute
    *
    * @param set_rank 
    * 
    */
    rank_= set_rank;
}

std::ostream & operator <<( std::ostream & out, const Category & cat ){
    /**
    * Overloading << operator for Category class
    *
    * @param set_rank 
    * 
    */
    out << "Category(" << cat.category_id_ << ":" << cat.rank_ << ")";
    return out;
}