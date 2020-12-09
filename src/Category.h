#include <iostream>

class Category : public AtomicMCDAObject{
    public : 

    Category(std::string category_id, int cat_rank);
    Category(const Category & category);
    ~Category() {};

    std::string getCategoryId() const;
    void setCategoryId(std::string set_cat_id);

    int getCategoryRank() const;
    void setCategoryRank(int set_rank);

    friend std::ostream& operator <<( std::ostream & out, const Category & category );

    private : 
        std::string category_id_;
        int rank_;
};