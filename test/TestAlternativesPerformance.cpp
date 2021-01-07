#include "../src/AlternativesPerformance.h"
#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestAlternativesPerformance, TestBaseConstructorWithoutMap) {
  Criteria crit = Criteria(2, "a");
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "test");
  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(
      os.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "test0, crit : a0, value : 0 ) Perf( name : test0, crit : a1, value : 0 "
      ") | Performance: Perf( name : test1, crit : a0, value : 0 ) Perf( name "
      ": test1, crit : a1, value : 0 ) | ], AlternativesAssignment{ test0->"
      " test1-> }");
  EXPECT_EQ(alt_perf.getMode(), "alt");
}

TEST(TestAlternativesPerformance, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}