#include "../../include/types/Perf.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestPerf, TestBaseConstructor) {
  Perf perf = Perf("test", "a", 0.6);
  std::ostringstream os;
  os << perf;
  EXPECT_EQ(os.str(), "Perf( name : test, crit : a, value : 0.6 )");

  perf = Perf("test", "b");
  std::ostringstream os2;
  os2 << perf;
  EXPECT_EQ(os2.str(), "Perf( name : test, crit : b, value : 0 )");
}

TEST(TestPerf, TestConstructorByCopy) {
  Perf perf = Perf("test", "a", 0.6);
  Perf perf_copied = Perf(perf);
  std::ostringstream os;
  os << perf_copied;
  EXPECT_EQ(os.str(), "Perf( name : test, crit : a, value : 0.6 )");
}

TEST(TestPerf, TestGetterSetter) {
  Perf perf = Perf("test", "a", 0.6);
  EXPECT_FLOAT_EQ(0.6, perf.getValue());
  EXPECT_EQ("test", perf.getName());
  EXPECT_EQ("a", perf.getCrit());

  perf.setValue(0.4);
  perf.setCrit("b");
  perf.setName("test2");

  EXPECT_FLOAT_EQ(0.4, perf.getValue());
  EXPECT_EQ("test2", perf.getName());
  EXPECT_EQ("b", perf.getCrit());
}
