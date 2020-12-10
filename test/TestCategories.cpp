#include "../src/Categories.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

int AtomicMCDAObject::nb_instances_{0};

TEST(TestCategories, TestCategoriesconstructor1) {
  Categories categories1(3, "hello");
  std::ostringstream os;
  os << category1;
  EXPECT_EQ(os.str(), "Categories(Category(id : hello0, rank : 0),Category(id "
                      ": hello1, rank : 1),Category(id : hello2, rank : 2) )");
}

TEST(TestCategories, TestCategoriesconstructor1_2) {
  Categories categories1(1, "hello");
  std::ostringstream os;
  os << category1;
  EXPECT_EQ(os.str(), "Categories(Category(id : hello0, rank : 0))");
}

TEST(TestCategories, TestCategoriesconstructor2) {
  Categories categories1(3);
  std::ostringstream os;
  os << category1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 0),Category(id "
                      ": cat1, rank : 1),Category(id : cat2, rank : 2) )");
}

TEST(TestCategories, TestCategoriesconstructor2_2) {
  Categories categories1(3);
  std::ostringstream os;
  os << category1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 0))");
}

TEST(TestCategories, TestConstructorByCopy) {
  Categories categories1("cat", 4);

  Categories catCopied = Categories(categories1);

  std::ostringstream os;
  os << catCopied;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 0),Category(id "
                      ": cat1, rank : 1),Category(id : cat2, rank : "
                      "2),Category(id : cat3, rank : 3) )");
}

TEST(TestCategories, TestOperatorSubscript) {
  Categories categories1(1);
  Category cat = categories1[0];

  std::ostringstream os;
  os << cat;
  EXPECT_EQ(os.str(), "Category(id : cat0, rank : 0))");
}

TEST(TestCategories, TestOperatorConstantSubscript) {
  const Categories categories1(1);
  Category cat = categories1[0];

  std::ostringstream os;
  os << cat;
  EXPECT_EQ(os.str(), "Category(id : cat0, rank : 0))");
}

TEST(TestCategories, TestGetRank) {
  const Categories categories1 = Categories(10);
  std::vector<int> rank_categories = categories1.getRankCategories();

  std::ostringstream os;
  os << rank_categories;
  EXPECT_EQ(os.str(), "[0 1 2 3 4 5 6 7 8 9 ]");
}

TEST(TestCategories, TestSetRank) {
  Categories categories1 = Categories(2);
  std::vector<int> rank_categories{4, 5};
  categories1.setRankCategories(rank_categories);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 4),Category(id "
                      ": cat1, rank : 5),");
}

TEST(TestCategories, TestSetRank2) {
  Categories categories1 = Categories(2);
  std::vector<int> rank_categories{4, 5};
  categories1.setRankCategories(rank_categories);
  categories1.setRankCategories();
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : cat0, rank : 0),Category(id "
                      ": cat1, rank : 1)");
}

TEST(TestCategories, TestGetId) {
  const Categories categories_ids = Categories(10);
  std::vector<int> rank_categories = categories1.getRankCategories();

  std::ostringstream os;
  os << rank_categories;
  EXPECT_EQ(os.str(), "[cat0 cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8 cat9 ]");
}

TEST(TestCategories, TestSetId) {
  Categories categories1 = Categories(2);
  std::string prefix = "fastpl";
  categories1.setIdCategories(prefix);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(), "Categories(Category(id : fastpl0, rank : 0),Category(id "
                      ": fastpl1, rank : 1),");
}

TEST(TestCategories, TestSetId2) {
  Categories categories1 = Categories(2);
  std::vector<std::string> set_category_ids = {"helloworld", "fastpl"};
  categories1.setIdCategories(rank_categories);
  std::ostringstream os;
  os << categories1;
  EXPECT_EQ(os.str(),
            "Categories(Category(id : helloworld, rank : 0),Category(id "
            ": fastpl, rank : 1)");
}

TEST(TestCategory, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}
