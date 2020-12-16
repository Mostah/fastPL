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
  EXPECT_EQ(perf_table.getMode(), "alt");
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
  EXPECT_EQ(perf_table.getMode(), "alt");
}

TEST(TestPerformanceTable, TestConstructorWithPerfVectErrors) {
  std::vector<Performance> perf_vect;

  try {
    PerformanceTable perf_table_0 = PerformanceTable(perf_vect);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The vector must contain performances."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }

  Criteria crit_a = Criteria(2, "a");
  Criteria crit_b = Criteria(2, "b");
  perf_vect.push_back(Performance("test0", crit_a));
  perf_vect.push_back(Performance("test1", crit_b));

  try {
    PerformanceTable perf_table_1 = PerformanceTable(perf_vect);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Each performance must be based on the same set of "
                          "criterion, in the same order."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }

  Criteria crit_c = Criteria(1, "c");
  perf_vect[1] = Performance("test2", crit_c);
  try {
    PerformanceTable perf_table_2 = PerformanceTable(perf_vect);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Each performance must be based on the same set of "
                          "criterion, in the same order."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }

  std::vector<Performance> perf_vect_err;
  perf_vect_err.push_back(Performance("test0", crit_c));
  perf_vect_err.push_back(Performance("test0", crit_c));

  try {
    PerformanceTable pt_err = PerformanceTable(perf_vect_err);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Each performance must have different ids."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
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
  EXPECT_EQ(perf_table.getMode(), "alt");
}

TEST(TestPerformanceTable, TestAccessOperator) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance("test0", crit));
  perf_vect.push_back(Performance("test1", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  std::vector<Perf> p0 = perf_table["test0"];
  std::ostringstream os;
  os << Performance(p0);
  EXPECT_EQ(os.str(), "Performance(Perf( name : test0, crit : a0, value : 0 "
                      "), Perf( name : test0, crit : a1, value : 0 ), )");

  try {
    std::vector<Perf> p_z = perf_table["z"];
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Row not found in performance table"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestPerformanceTable, TestGetPerf) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance("test0", crit));
  perf_vect.push_back(Performance("test1", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  Perf p = perf_table.getPerf("test1", "a0");
  std::ostringstream os;
  os << p;
  EXPECT_EQ(os.str(), "Perf( name : test1, crit : a0, value : 0 )");

  try {
    Perf p = perf_table.getPerf("test", "a0");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Name not found in performance table"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }

  try {
    Perf p = perf_table.getPerf("test0", "a");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("Criterion not found in performance table"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestPerformanceTable, TestChangeMode) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  perf_vect.push_back(Performance("test0", crit));
  perf_vect.push_back(Performance("test1", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  perf_table.changeMode("crit");
  std::ostringstream os;
  os << perf_table;
  EXPECT_EQ(
      os.str(),
      "PerformanceTable(Performance(Perf( name : test0, crit : a0, value : 0 "
      "), Perf( name : test1, crit : a0, value : 0 ), ), Performance(Perf( "
      "name : test0, crit : a1, value : 0 ), Perf( name : test1, crit : a1, "
      "value : 0 ), ), )");
  EXPECT_EQ(perf_table.getMode(), "crit");

  perf_table.changeMode("alt");
  std::ostringstream os2;
  os2 << perf_table;
  EXPECT_EQ(
      os2.str(),
      "PerformanceTable(Performance(Perf( name : test0, crit : a0, value : 0 "
      "), Perf( name : test0, crit : a1, value : 0 ), ), Performance(Perf( "
      "name : test1, crit : a0, value : 0 ), Perf( name : test1, crit : a1, "
      "value : 0 ), ), )");
  EXPECT_EQ(perf_table.getMode(), "alt");

  try {
    perf_table.changeMode("zzz");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Mode must be alt or crit."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestPerformanceTable, TestSort) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  std::vector<float> given_perf0 = {0.8, 0.4};
  std::vector<float> given_perf1 = {0.2, 0.6};
  perf_vect.push_back(Performance("test0", crit, given_perf0));
  perf_vect.push_back(Performance("test1", crit, given_perf1));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  perf_table.sort("alt");
  std::ostringstream os;
  os << perf_table;
  EXPECT_EQ(
      os.str(),
      "PerformanceTable(Performance(Perf( name : test0, crit : a1, value : 0.4 "
      "), Perf( name : test0, crit : a0, value : 0.8 ), ), Performance(Perf( "
      "name : test1, crit : a0, value : 0.2 ), Perf( name : test1, crit : a1, "
      "value : 0.6 ), ), )");
  EXPECT_EQ(perf_table.getMode(), "alt");

  perf_table = PerformanceTable(perf_vect);
  perf_table.sort("crit");
  std::ostringstream os2;
  os2 << perf_table;
  EXPECT_EQ(
      os2.str(),
      "PerformanceTable(Performance(Perf( name : test1, crit : a0, value : 0.2 "
      "), Perf( name : test0, crit : a0, value : 0.8 ), ), Performance(Perf( "
      "name : test0, crit : a1, value : 0.4 ), Perf( name : test1, crit : a1, "
      "value : 0.6 ), ), )");
  EXPECT_EQ(perf_table.getMode(), "crit");

  try {
    perf_table.changeMode("zzz");
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Mode must be alt or crit."));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }
}

TEST(TestPerformanceTable, TestGetAltBetween) {
  std::vector<Performance> perf_vect;
  Criteria crit = Criteria(2, "a");
  std::vector<float> given_perf0 = {0.2, 0};
  std::vector<float> given_perf1 = {0.8, 1};
  std::vector<float> given_perf2 = {0.4, 0.4};
  std::vector<float> given_perf3 = {0.6, 0.6};
  perf_vect.push_back(Performance("test0", crit, given_perf0));
  perf_vect.push_back(Performance("test1", crit, given_perf1));
  perf_vect.push_back(Performance("test2", crit, given_perf2));
  perf_vect.push_back(Performance("test3", crit, given_perf3));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  try {
    perf_table.getAltBetween("a0", 0, 1);
    FAIL() << "should have throw domain error.";
  } catch (std::domain_error const &err) {
    EXPECT_EQ(err.what(),
              std::string("Mode must be crit but is currently set to alt."));
  } catch (...) {
    FAIL() << "should have throw domain error.";
  }

  perf_table.sort("crit");
  try {
    perf_table.getAltBetween("a0", 1, 0);
    FAIL() << "should have throw invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("Sup must be greater (>) than inf"));
  } catch (...) {
    FAIL() << "should have throw invalid argument.";
  }

  EXPECT_EQ(0, perf_table.getAltBetween("a0", 0.25, 0.3).size());
  EXPECT_EQ(0, perf_table.getAltBetween("a0", 0.81, 1).size());

  std::ostringstream os;
  std::vector<Perf> p0 = perf_table.getAltBetween("a0", 0.1, 0.5);
  os << p0;
  EXPECT_EQ(os.str(), "[Perf( name : test0, crit : a0, value : 0.2 "
                      "),Perf( name : test2, crit : a0, value : 0.4 )]");

  std::ostringstream os2;
  std::vector<Perf> p1 = perf_table.getAltBetween("a1", 0.5, 1);
  os2 << p1;
  EXPECT_EQ(os2.str(), "[Perf( name : test3, crit : a1, value : 0.6 "
                       "),Perf( name : test1, crit : a1, value : 1 )]");
}

TEST(TestPerformanceTable, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}