#include "../src/AlternativesPerformance.h"
#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestAlternativesPerformance, TestConstructorBaseConstructor) {
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

  std::map<std::string, std::string> map =
      std::map<std::string, std::string>{{"test0", "cat0"}, {"test1", "cat1"}};
  AlternativesPerformance alt_perf2 =
      AlternativesPerformance(2, crit, "test", map);
  std::ostringstream os2;
  os2 << alt_perf2;
  EXPECT_EQ(
      os2.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "test0, crit : a0, value : 0 ) Perf( name : test0, crit : a1, value : 0 "
      ") | Performance: Perf( name : test1, crit : a0, value : 0 ) Perf( name "
      ": test1, crit : a1, value : 0 ) | ], AlternativesAssignment{ test0->cat0"
      " test1->cat1 }");
  EXPECT_EQ(alt_perf2.getMode(), "alt");
}

TEST(TestAlternativesPerformance, TestConstructorMapErrors) {
  Criteria crit = Criteria(2, "a");

  std::map<std::string, std::string> map_err =
      std::map<std::string, std::string>{{"test10", "cat0"}, {"test5", "cat1"}};
  try {
    AlternativesPerformance alt_perf_err =
        AlternativesPerformance(2, crit, "test", map_err);
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The alternatives in the map should be "
                                      "present in the performance table."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfVect) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance(crit, "test0"));
  perf_vect.push_back(Performance(crit, "test1"));

  AlternativesPerformance alt_perf = AlternativesPerformance(perf_vect);
  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(os.str(),
            "AlternativesPerformance( PerformanceTable[ Performance: Perf( "
            "name : test0, crit : a0, value "
            ": 0 ) Perf( name : test0, crit : a1, value : 0 ) | "
            "Performance: Perf( name : test1, crit : a0, value : 0 ) Perf( "
            "name : test1, crit : a1, value : 0 ) | ], AlternativesAssignment{ "
            "test0->"
            " test1-> }");
  EXPECT_EQ(alt_perf.getMode(), "alt");

  std::map<std::string, std::string> map =
      std::map<std::string, std::string>{{"test0", "cat0"}, {"test1", "cat1"}};

  AlternativesPerformance alt_perf2 = AlternativesPerformance(perf_vect, map);
  std::ostringstream os2;
  os2 << alt_perf2;
  EXPECT_EQ(os2.str(),
            "AlternativesPerformance( PerformanceTable[ Performance: Perf( "
            "name : test0, crit : a0, value "
            ": 0 ) Perf( name : test0, crit : a1, value : 0 ) | "
            "Performance: Perf( name : test1, crit : a0, value : 0 ) Perf( "
            "name : test1, crit : a1, value : 0 ) | ], AlternativesAssignment{ "
            "test0->cat0"
            " test1->cat1 }");
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfVectMapErrors) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance(crit, "test0"));
  perf_vect.push_back(Performance(crit, "test1"));
  std::map<std::string, std::string> map =
      std::map<std::string, std::string>{{"test12", "cat0"}, {"test1", "cat1"}};
  try {
    AlternativesPerformance alt_perf = AlternativesPerformance(perf_vect, map);
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The alternatives in the map should be "
                                      "present in the performance table."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfTable) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance(crit, "test0"));
  perf_vect.push_back(Performance(crit, "test1"));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table);

  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(os.str(),
            "AlternativesPerformance( PerformanceTable[ Performance: Perf( "
            "name : test0, crit : a0, value "
            ": 0 ) Perf( name : test0, crit : a1, value : 0 ) | "
            "Performance: Perf( name : test1, crit : a0, value : 0 ) Perf( "
            "name : test1, crit : a1, value : 0 ) | ], AlternativesAssignment{ "
            "test0-> test1-> }");
  EXPECT_EQ(alt_perf.getMode(), "alt");

  std::map<std::string, std::string> map =
      std::map<std::string, std::string>{{"test0", "cat0"}, {"test1", "cat1"}};
  AlternativesPerformance alt_perf2 = AlternativesPerformance(perf_table, map);
  std::ostringstream os2;
  os2 << alt_perf2;
  EXPECT_EQ(os2.str(),
            "AlternativesPerformance( PerformanceTable[ Performance: Perf( "
            "name : test0, crit : a0, value "
            ": 0 ) Perf( name : test0, crit : a1, value : 0 ) | "
            "Performance: Perf( name : test1, crit : a0, value : 0 ) Perf( "
            "name : test1, crit : a1, value : 0 ) | ], AlternativesAssignment{ "
            "test0->cat0"
            " test1->cat1 }");
  EXPECT_EQ(alt_perf2.getMode(), "alt");
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfTableMapErrors) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance(crit, "test0"));
  perf_vect.push_back(Performance(crit, "test1"));
  PerformanceTable perf_table = PerformanceTable(perf_vect);
  std::map<std::string, std::string> map =
      std::map<std::string, std::string>{{"test12", "cat0"}, {"test1", "cat1"}};

  try {
    AlternativesPerformance alt_perf = AlternativesPerformance(perf_table, map);
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The alternatives in the map should be "
                                      "present in the performance table."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}

TEST(TestAlternativesPerformance, TestCopyConstructor) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance(crit, "test0"));
  perf_vect.push_back(Performance(crit, "test1"));
  PerformanceTable perf_table = PerformanceTable(perf_vect);
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table);

  std::ostringstream os;
  std::ostringstream os2;

  AlternativesPerformance alt_perf2 = AlternativesPerformance(alt_perf);
  os << alt_perf;
  os2 << alt_perf2;
  EXPECT_EQ(os.str(), os2.str());
}

TEST(TestAlternativesPerformance, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}