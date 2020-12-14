#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestPerformanceTable, TestBaseConstructor) {
  Criteria crit = Criteria(2, "a");
  PerformanceTable perf_table = PerformanceTable("test", 2, crit);
  std::ostringstream os;
  os << perf_table;
  EXPECT_EQ(os.str(),
            "PerformanceTable(Performance(Perf( name : test0, crit : a0, value "
            ": 0 ), Perf( name : test0, crit : a1, value : 0 ), ), "
            "Performance(Perf( name : test1, crit : a0, value : 0 ), Perf( "
            "name : test1, crit : a1, value : 0 ), ), )");
}

TEST(TestPerformanceTable, TestConstructorWithPerfVect) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance("test0", crit));
  perf_vect.push_back(Performance("test1", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  std::ostringstream os;
  os << perf_table;
  EXPECT_EQ(os.str(),
            "PerformanceTable(Performance(Perf( name : test0, crit : a0, value "
            ": 0 ), Perf( name : test0, crit : a1, value : 0 ), ), "
            "Performance(Perf( name : test1, crit : a0, value : 0 ), Perf( "
            "name : test1, crit : a1, value : 0 ), ), )");
}

TEST(TestPerformanceTable, TestConstructorByCopy) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance("test0", crit));
  perf_vect.push_back(Performance("test1", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  std::ostringstream os;

  PerformanceTable perf_table2 = PerformanceTable(perf_table);
  os << perf_table2;
  EXPECT_EQ(
      os.str(),
      "PerformanceTable(Performance(Perf( name : test0, crit : a0, value : 0 "
      "), Perf( name : test0, crit : a1, value : 0 ), ), Performance(Perf( "
      "name : test1, crit : a0, value : 0 ), Perf( name : test1, crit : a1, "
      "value : 0 ), ), )");
}

TEST(TestPerformanceTable, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}