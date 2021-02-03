#include "../../include/types/AlternativesPerformance.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"

#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestAlternativesPerformance, TestConstructorBaseConstructor) {
  Criteria crit = Criteria(2, "crit");
  Category cat0 = Category("cat0", 0);

  AlternativesPerformance alt_perf = AlternativesPerformance(1, crit, "a");
  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(
      os.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "a0, crit : crit0, value : 0 ) Perf( name : a0, crit : crit1, value : 0 "
      ") | ], AlternativesAssignment{ {, (a0: Category(id : , rank : -1)),}");
  EXPECT_EQ(alt_perf.getMode(), "alt");

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}};
  AlternativesPerformance alt_perf2 =
      AlternativesPerformance(1, crit, "a", map);
  std::ostringstream os2;
  os2 << alt_perf2;
  EXPECT_EQ(os2.str(),
            "AlternativesPerformance( PerformanceTable[ Performance: Perf( "
            "name : a0, crit : crit0, value : 0 ) Perf( name : a0, crit : "
            "crit1, value : 0 ) | ], AlternativesAssignment{ {, (a0: "
            "Category(id : cat0, rank : 0)),}");
  EXPECT_EQ(alt_perf2.getMode(), "alt");
}

TEST(TestAlternativesPerformance, TestConstructorMapErrors) {
  Criteria crit = Criteria(2, "crit");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);

  std::unordered_map<std::string, Category> map_err =
      std::unordered_map<std::string, Category>{{"a10", cat0}, {"a5", cat1}};
  try {
    AlternativesPerformance alt_perf_err =
        AlternativesPerformance(2, crit, "a", map_err);
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The alternatives in the map should be "
                                      "present in the performance table."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfVect) {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(2, "crit");
  perf_vect.push_back(createVectorPerfWithNoPerf("a0", crit));
  // perf_vect.push_back(Performance(crit, "a1"));

  AlternativesPerformance alt_perf = AlternativesPerformance(perf_vect);
  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(
      os.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "a0, crit : crit0, value : 0 ) Perf( name : a0, crit : crit1, value : 0 "
      ") | ], AlternativesAssignment{ {, (a0: Category(id : , rank : -1)),}");
  EXPECT_EQ(alt_perf.getMode(), "alt");

  Category cat0 = Category("cat0", 0);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}};

  AlternativesPerformance alt_perf2 = AlternativesPerformance(perf_vect, map);
  std::ostringstream os2;
  os2 << alt_perf2;
  EXPECT_EQ(os2.str(),
            "AlternativesPerformance( PerformanceTable[ Performance: Perf( "
            "name : a0, crit : crit0, value : 0 ) Perf( name : a0, crit : "
            "crit1, value : 0 ) | ], AlternativesAssignment{ {, (a0: "
            "Category(id : cat0, rank : 0)),}");
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfVectMapErrors) {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(2, "crit");
  perf_vect.push_back(createVectorPerfWithNoPerf("a0", crit));
  perf_vect.push_back(createVectorPerfWithNoPerf("a1", crit));
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);

  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a12", cat0}, {"a1", cat1}};

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
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(2, "crit");
  perf_vect.push_back(createVectorPerfWithNoPerf("a0", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table);

  std::ostringstream os;
  os << alt_perf;
  EXPECT_EQ(
      os.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "a0, crit : crit0, value : 0 ) Perf( name : a0, crit : crit1, value : 0 "
      ") | ], AlternativesAssignment{ {, (a0: Category(id : , rank : -1)),}");
  EXPECT_EQ(alt_perf.getMode(), "alt");

  Category cat0 = Category("cat0", 0);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}};
  AlternativesPerformance alt_perf2 = AlternativesPerformance(perf_table, map);
  std::ostringstream os2;
  os2 << alt_perf2;
  EXPECT_EQ(
      os2.str(),
      "AlternativesPerformance( PerformanceTable[ Performance: Perf( name : "
      "a0, crit : crit0, value : 0 ) Perf( name : a0, crit : crit1, value : 0 "
      ") | ], AlternativesAssignment{ {, (a0: Category(id : cat0, rank : "
      "0)),}");
  EXPECT_EQ(alt_perf2.getMode(), "alt");
}

TEST(TestAlternativesPerformance, TestConstructorWithPerfTableMapErrors) {
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(2, "crit");
  perf_vect.push_back(createVectorPerfWithNoPerf("a0", crit));
  perf_vect.push_back(createVectorPerfWithNoPerf("a1", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);

  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a12", cat0}, {"a1", cat1}};

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
  std::vector<std::vector<Perf>> perf_vect;
  Criteria crit = Criteria(2, "crit");
  perf_vect.push_back(createVectorPerfWithNoPerf("a0", crit));
  PerformanceTable perf_table = PerformanceTable(perf_vect);
  AlternativesPerformance alt_perf = AlternativesPerformance(perf_table);

  std::ostringstream os;
  std::ostringstream os2;

  AlternativesPerformance alt_perf2 = AlternativesPerformance(alt_perf);
  os << alt_perf;
  os2 << alt_perf2;
  EXPECT_EQ(os.str(), os2.str());
}

TEST(TestAlternativesPerformance, TestGetAssignmentMap) {
  Criteria crit = Criteria(2, "crit");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}, {"a1", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "a", map);

  std::unordered_map<std::string, Category> map2 =
      alt_perf.getAlternativesAssignments();

  EXPECT_EQ(map, map2);
}

TEST(TestAlternativesPerformance, TestSetAssignmentMap) {
  Criteria crit = Criteria(2, "crit");
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "a");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}, {"a1", cat1}};

  alt_perf.setAlternativesAssignments(map);

  EXPECT_EQ(map, alt_perf.getAlternativesAssignments());
}

TEST(TestAlternativesPerformance, TestSetAssignmentMapErrors) {
  Criteria crit = Criteria(2, "crit");
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "a");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"test_error", cat0},
                                                {"a1", cat1}};

  try {
    alt_perf.setAlternativesAssignments(map);
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The alternatives in the map should be "
                                      "present in the performance table."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}

TEST(TestAlternativesPerformance, TestGetAlternativeAssignment) {
  Criteria crit = Criteria(2, "crit");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}, {"a1", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "a", map);

  Category alt_a0_assignment = alt_perf.getAlternativeAssignment("a0");

  EXPECT_EQ(alt_a0_assignment, cat0);
}

TEST(TestAlternativesPerformance, TestSetAlternativeAssignment) {
  Criteria crit = Criteria(2, "crit");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}, {"a1", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "a", map);

  Category cat3 = Category("cat3", 3);
  alt_perf.setAlternativeAssignment("a0", cat3);

  EXPECT_EQ(alt_perf.getAlternativeAssignment("a0"), cat3);
}

TEST(TestAlternativesPerformance, TestSetAlternativeAssignmentError) {
  Criteria crit = Criteria(2, "crit");
  Category cat0 = Category("cat0", 0);
  Category cat1 = Category("cat1", 1);
  std::unordered_map<std::string, Category> map =
      std::unordered_map<std::string, Category>{{"a0", cat0}, {"a1", cat1}};
  AlternativesPerformance alt_perf = AlternativesPerformance(2, crit, "a", map);

  Category cat3 = Category("cat3", 3);
  try {
    alt_perf.setAlternativeAssignment("testerror0", cat3);
    FAIL() << "should have thrown invalid argument.";
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(), std::string("The alternatives in the map should be "
                                      "present in the performance table."));
  } catch (...) {
    FAIL() << "should have thrown invalid argument.";
  }
}