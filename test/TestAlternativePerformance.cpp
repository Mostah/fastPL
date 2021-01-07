#include "../src/AlternativePerformance.h"
#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestAlternativePerformance, TestBaseConstructorWithoutMap) {
  Criteria crit = Criteria(2, "a");
  AlternativePerformance alt_perf = AlternativePerformance(2, crit, "test");
  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(os.str(),
            "PerformanceTable(Performance(Perf( name : test0, crit : a0, value "
            ": 0 ), Perf( name : test0, crit : a1, value : 0 ), ), "
            "Performance(Perf( name : test1, crit : a0, value : 0 ), Perf( "
            "name : test1, crit : a1, value : 0 ), ), )");
  EXPECT_EQ(alt_perf.getMode(), "alt");
}

TEST(TestAlternativePerformance, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}