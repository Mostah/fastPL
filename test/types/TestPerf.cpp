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

TEST(TestPerf, TestOperatorEqual) {
  Perf perf = Perf("test", "a", 0.6);
  Perf perf2 = Perf("test", "a", 0.6);
  Perf perf3 = Perf("test", "a1", 0.6);
  EXPECT_EQ(perf3 == perf2, 0);
  EXPECT_EQ(perf == perf2, 1);
}
