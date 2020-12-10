#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestPerformance, TestBaseConstructor) {
  std::string id = "test";
  Criteria crit = Criteria(2, "test");
  Performance perf = Performance(id, crit);
  std::ostringstream os;
  os << perf;
  EXPECT_EQ(
      os.str(),
      "Performance({ cat : test0, perf : 0 }, { cat : test1, perf : 0 }, )");
}

TEST(TestPerformance, TestConstructorWithPerfVect) {
  std::string id = "test";
  Criteria crit = Criteria(2, "test");
  std::vector<float> given_perf = {0.4, 0.6};
  Performance perf = Performance(id, crit, given_perf);
  std::ostringstream os;
  os << perf;
  EXPECT_EQ(os.str(), "Performance({ cat : test0, perf : 0.4 }, { cat : test1, "
                      "perf : 0.6 }, )");
}

TEST(TestPerformance, TestConstructorByCopy) {
  std::string id = "test";
  Criteria crit = Criteria(2, "test");
  std::vector<float> given_perf = {0.4, 0.6};
  Performance perf = Performance(id, crit, given_perf);

  Performance perf_copied = Performance(perf);
  std::ostringstream os;
  os << perf_copied;
  EXPECT_EQ(os.str(), "Performance({ cat : test0, perf : 0.4 }, { cat : test1, "
                      "perf : 0.6 }, )");
}

TEST(TestPerformance, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}