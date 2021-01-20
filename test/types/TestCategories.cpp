#include "../../include/types/Categories.h"
#include "../../include/utils.h"
#include "gtest/gtest.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

TEST(TestCategories, TestCategoriesconstructor1Plural) {
  Categories categories1 = Categories(3, "hello");
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(),
            "Categories(Category(id : hello0, rank : 0), Category(id : hello1, "
            "rank : 1), Category(id : hello2, rank : 2))");
}

TEST(TestCategories, TestCategoriesconstructor1WithNumbersAndIds) {
  Categories categories1 = Categories(1, "hello");
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : hello0, rank : 0))");
}

TEST(TestCategories, TestCategoriesconstructor1WithoutId) {
  Categories categories1 = Categories(3);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 0), Category(id : "
                      "cat1, rank : 1), Category(id : cat2, rank : 2))");
}

TEST(TestCategories, TestCategoriesconstructor2) {
  std::vector<std::string> vect_cat_ids = {"hello0", "hello1", "hello2"};
  Categories categories1 = Categories(vect_cat_ids);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(),
            "Categories(Category(id : hello0, rank : 0), Category(id : "
            "hello1, rank : 1), Category(id : hello2, rank : 2))");
}

TEST(TestCategories, TestConstructorByCopy) {
  Categories categories1 = Categories(4, "cat");

  Categories catCopied = Categories(categories1);

  std::ostringstream os;
  os << catCopied;
  EXPECT_EQ(
      os.str(),
      "Categories(Category(id : cat0, rank : 0), Category(id : cat1, rank : "
      "1), Category(id : cat2, rank : 2), Category(id : cat3, rank : 3))");
}

TEST(TestCategories, TestOperatorSubscript) {
  Categories categories1 = Categories(1);
  Category cat = categories1[0];

  std::ostringstream os;
  os << cat;
  EXPECT_EQ(os.str(), "Category(id : cat0, rank : 0)");
}

TEST(TestCategories, TestOperatorConstantSubscript) {
  const Categories categories1 = Categories(1);
  Category cat = categories1[0];

  std::ostringstream os;
  os << cat;
  EXPECT_EQ(os.str(), "Category(id : cat0, rank : 0)");
}

TEST(TestCategories, TestGetRank) {
  Categories categories1 = Categories(10);
  std::vector<int> rank_categories = categories1.getRankCategories();

  std::ostringstream os;
  os << rank_categories;
  EXPECT_EQ(os.str(), "[0,1,2,3,4,5,6,7,8,9]");
}

TEST(TestCategories, TestSetRank) {
  Categories categories1 = Categories(2);
  std::vector<int> rank_categories{4, 5};
  categories1.setRankCategories(rank_categories);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 4), Category(id : "
                      "cat1, rank : 5))");
}

TEST(TestCategories, TestSetRank2) {
  Categories categories1 = Categories(2);
  std::vector<int> rank_categories{4, 5};
  categories1.setRankCategories(rank_categories);
  categories1.setRankCategories();
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 0), Category(id : "
                      "cat1, rank : 1))");
}

TEST(TestCategories, TestGetId) {
  Categories categories1 = Categories(10, "cat");
  std::vector<std::string> categories_ids = categories1.getIdCategories();

  std::ostringstream os;
  os << categories_ids;
  EXPECT_EQ(os.str(), "[cat0,cat1,cat2,cat3,cat4,cat5,cat6,cat7,cat8,cat9]");
}

TEST(TestCategories, TestSetId) {
  Categories categories1 = Categories(2);
  std::string prefix = "fastpl";
  categories1.setIdCategories(prefix);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : fastpl0, rank : 0), "
                      "Category(id : fastpl1, rank : 1))");
}

TEST(TestCategories, TestSetId2) {
  Categories categories1 = Categories(2);
  std::vector<std::string> set_category_ids = {"helloworld", "fastpl"};
  categories1.setIdCategories(set_category_ids);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(),
            "Categories(Category(id : helloworld, rank : 0), Category(id "
            ": fastpl, rank : 1))");
}

TEST(TestCategories, TestGetCategoryOfRank) {
  Categories categories1 = Categories(2);
  std::vector<std::string> set_category_ids = {"helloworld", "fastpl"};
  categories1.setIdCategories(set_category_ids);

  Category cat0 = categories1.getCategoryOfRank(0);
  Category cat1 = categories1.getCategoryOfRank(1);

  std::ostringstream os1;
  os1 << cat0;
  EXPECT_EQ(os1.str(), "Category(id : helloworld, rank : 0)");
  std::ostringstream os2;
  os2 << cat1;
  EXPECT_EQ(os2.str(), "Category(id : fastpl, rank : 1)");
}