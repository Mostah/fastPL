#include "../src/Criteria.h"
#include "../src/MRSortModel.h"
#include "../src/PerformanceTable.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestMRSortModel, TestBaseConstructor) {
  Criteria crit = Criteria(1);
  PerformanceTable prof = PerformanceTable(1, crit);

  MRSortModel mrsort = MRSortModel(crit, prof, 0.5);
  std::ostringstream os;
  os << mrsort;
  EXPECT_EQ(os.str(),
            "Model( id : model, lambda : 0.5, crit : Criteria(Criterion(id "
            ": crit0, name : , direction : +, weight : 0), ), "
            "profiles : PerformanceTable(Performance(Perf( name : "
            "alt0, crit : crit0, value : 0 ), ), ))");
}

TEST(TestMRSort, TestAllInstancesDestroyed) {
  EXPECT_EQ(AtomicMCDAObject::get_nb_instances(), 0);
}