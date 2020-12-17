#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestPerformance, TestBaseConstructor) {
  std::string id = "test";
  Criteria crit = Criteria(2, "a");
  Performance perf = Performance(id, crit);
  std::ostringstream os;
  os << perf;
  EXPECT_EQ(os.str(), "Performance(Perf( name : test, crit : a0, value : 0 ), "
                      "Perf( name : test, crit : a1, value : 0 ), )");
}

TEST(TestPerformance, TestConstructorWithPerfFloatVect) {
  std::string id = "test";
  Criteria crit = Criteria(2, "a");
  std::vector<float> given_perf = {0.4, 0.6};
  Performance perf = Performance(id, crit, given_perf);
  std::ostringstream os;
  os << perf;
  EXPECT_EQ(os.str(),
            "Performance(Perf( name : test, crit : a0, value : 0.4 ), "
            "Perf( name : test, crit : a1, value : 0.6 ), )");
}

TEST(TestPerformance, TestConstructorWithPerfVect) {
  std::vector<Perf> p = {Perf("test", "a", 0.1), Perf("test", "b", 0.1)};
  Performance perf = Performance(p);
  std::ostringstream os;
  os << perf;
  EXPECT_EQ(os.str(), "Performance(Perf( name : test, crit : a, value : 0.1 ), "
                      "Perf( name : test, crit : b, value : 0.1 ), )");
  try {
    std::vector<Perf> p2 = {Perf("test", "a", 0.1), Perf("zzz", "b", 0.1)};
    Performance perf2 = Performance(p2);
    FAIL();
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("All Perf in the vector must have the same name"));
  }
}

TEST(TestPerformance, TestConstructorByCopy) {
  std::string id = "test";
  Criteria crit = Criteria(2, "a");
  std::vector<float> given_perf = {0.4, 0.6};
  Performance perf = Performance(id, crit, given_perf);

  Performance perf_copied = Performance(perf);
  std::ostringstream os;
  os << perf_copied;
  EXPECT_EQ(os.str(),
            "Performance(Perf( name : test, crit : a0, value : 0.4 ), "
            "Perf( name : test, crit : a1, value : 0.6 ), )");
}

TEST(TestPerformance, TestAccessOperator) {
  std::string id = "test";
  Criteria crit = Criteria(2, "a");
  Performance perf = Performance(id, crit);
  Perf p_a0 = perf["a0"];
  std::ostringstream os;
  os << p_a0;
  EXPECT_EQ(os.str(), "Perf( name : test, crit : a0, value : 0 )");
  try {
    Perf p_z = perf["z"];
    FAIL();
  } catch (std::invalid_argument const &err) {
    EXPECT_EQ(err.what(),
              std::string("criterion not found in performance vector"));
  }
}

TEST(TestPerformance, TestGetCriteriaVect) {
  Criteria crit = Criteria(2, "a");
  Performance perf = Performance("test", crit);
  std::vector<std::string> crit_vect = {"a0", "a1"};
  EXPECT_EQ(perf.getCriterionIds(), crit_vect);
}

TEST(TestPerformance, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}