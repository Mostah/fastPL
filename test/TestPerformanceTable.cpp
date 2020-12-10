#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestPerformanceTable, TestBaseConstructor) {
  Criteria crit = Criteria(2, "test");
  PerformanceTable perf_table = PerformanceTable("", 2, crit);
  std::ostringstream os;
  os << perf_table;
  EXPECT_EQ(os.str(), "PerformanceTable(Performance({ cat : test0, perf : 0 }, "
                      "{ cat : test1, perf : 0 }, ), Performance({ cat : "
                      "test0, perf : 0 }, { cat : test1, perf : 0 }, ), )");
}

TEST(TestPerformanceTable, TestConstructorWithPerfVect) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "test");
  perf_vect.push_back(Performance("", crit));
  perf_vect.push_back(Performance("", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  std::ostringstream os;
  os << perf_table;
  EXPECT_EQ(
      os.str(),
      "PerformanceTable(Performance({ cat : test0, perf : 0 }, { cat : test1, "
      "perf : 0 }, ), Performance({ cat : test0, perf : 0 }, { cat : test1, "
      "perf : 0 }, ), )");
}

TEST(TestPerformanceTable, TestConstructorByCopy) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "test");
  perf_vect.push_back(Performance("", crit));
  perf_vect.push_back(Performance("", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  std::ostringstream os;

  PerformanceTable perf_table2 = PerformanceTable(perf_table);
  os << perf_table2;
  EXPECT_EQ(
      os.str(),
      "PerformanceTable(Performance({ cat : test0, perf : 0 }, { cat : test1, "
      "perf : 0 }, ), Performance({ cat : test0, perf : 0 }, { cat : test1, "
      "perf : 0 }, ), )");
}

TEST(TestPerformanceTable, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}