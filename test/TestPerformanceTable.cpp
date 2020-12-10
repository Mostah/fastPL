#include "../src/Criteria.h"
#include "../src/Performance.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

// TEST(TestPerformanceTable, TestBaseConstructor) {
//   std::string id = "test";
//   Criteria crit = Criteria(2, "test");
//   Performance perf = Performance(id, crit);
//   std::ostringstream os;
//   os << perf;
//   EXPECT_EQ(
//       os.str(),
//       "Performance({ cat : test0, perf : 0 }, { cat : test1, perf : 0 }, )");
// }

// TEST(TestPerformanceTable, TestConstructorWithPerfVect) {
//   std::vector<Performance> perf_vect;
//   Criteria crit = Criteria(2, "test");
//   perf_vect.push_back(Performance("", crit));
//   perf_vect.push_back(Performance("", crit));
//   PerformanceTable perf_table = PerformanceTable(perf_vect);

//   std::ostringstream os;
//   os << perf_table;
//   EXPECT_EQ(
//       os.str(),
//       "PerformanceTable(Performance({ cat : test0, perf : 0 }, { cat : test1,
//       " "perf : 0 }, ), Performance({ cat : test0, perf : 0 }, { cat : test1,
//       " "perf : 0 }, )");
// }

// TEST(TestPerformanceTable, TestConstructorByCopy) {
//   std::string id = "test";
//   Criteria crit = Criteria(2, "test");
//   std::vector<float> given_perf = {0.4, 0.6};
//   Performance perf = Performance(id, crit, given_perf);

//   Performance perf_copied = Performance(perf);
//   std::ostringstream os;
//   os << perf_copied;
//   EXPECT_EQ(os.str(), "Performance({ cat : test0, perf : 0.4 }, { cat :
//   test1, "
//                       "perf : 0.6 }, )");
// }

// TEST(TestPerformanceTable, TestAllInstancesDestroyed) {
//   EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
// }