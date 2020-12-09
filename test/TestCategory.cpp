#include "../src/Category.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>


TEST(TestCategory, TestCategory) {
  Category category1("cat0", 0);
  std::ostringstream os;
  os << category1;
  EXPECT_EQ(os.str(), "Category(cat0:0)");
}

TEST(TestCategory, TestCategory2) {
  Category category1("cat12", 12);
  std::ostringstream os;
  os << category1;
  EXPECT_EQ(os.str(), "Category(cat12:12)");
}


TEST(TestCategory, TestConstructorByCopy) {
  Category category1("cat1", 13);

  Category catCopied = Category(category1);

  std::ostringstream os;
  os << catCopied;
  EXPECT_EQ(os.str(), "Category(cat1:13)");
}

TEST(TestCategory, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}