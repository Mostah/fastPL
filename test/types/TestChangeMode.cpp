#include "../../include/types/Perf.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"

#include "gtest/gtest.h"
#include <sstream>
#include <utility>

// Checking PerformanceTable changeModes

TEST(TestPerformanceTable, TestChangeMode1) {
  std::vector<std::vector<Perf>> perf_vect;

  Criteria crit = Criteria(2, "crit");
  std::vector<float> given_perf0 = {0.6, 1};
  std::vector<float> given_perf1 = {0.8, 0.1};
  perf_vect.push_back(createVectorPerf("alt0", crit, given_perf0));
  perf_vect.push_back(createVectorPerf("alt1", crit, given_perf1));
  PerformanceTable perf_table = PerformanceTable(perf_vect);
  std::cout << perf_table << std::endl;
  std::cout << "\n";

  perf_table.changeMode("crit");
  std::cout << "Mode has changed to crit" << std::endl;
  std::cout << perf_table << std::endl;
  std::cout << "\n";

  std::cout << "Mode has changed to alt" << std::endl;
  perf_table.changeMode("alt");
  std::cout << perf_table << std::endl;
  std::cout << "\n";

  std::cout << "Mode has changed to crit" << std::endl;
  perf_table.changeMode("crit");
  std::cout << perf_table << std::endl;
  std::cout << "\n";

  std::cout << "Mode has changed to alt" << std::endl;
  perf_table.changeMode("alt");
  std::cout << perf_table << std::endl;
  std::cout << "\n";
  //////////

  std::cout << "Creating a crit mode PT" << std::endl;
  std::vector<std::vector<Perf>> perf_vect2;
  std::vector<std::vector<float>> values;
  std::vector<float> p = {0.6, 0.8};
  std::vector<float> p2 = {1, 0.1};
  values.push_back(p);
  values.push_back(p2);
  // creating perf vect for crit mode
  for (int i = 0; i < crit.getCriterionVect().size(); i++) {
    std::vector<Perf> aux;
    for (int j = 0; j < values.size(); j++) {
      aux.push_back(
          Perf("alt" + std::to_string(j), crit[i].getId(), values[i][j]));
    }
    perf_vect2.push_back(aux);
  }

  PerformanceTable perf_table2 = PerformanceTable(perf_vect2, "crit");
  std::cout << perf_table2 << std::endl;
  std::cout << "\n";

  std::cout << "Mode has changed to alt" << std::endl;
  perf_table2.changeMode("alt");
  std::cout << perf_table2 << std::endl;
  std::cout << "\n";

  std::cout << "Mode has changed to crit" << std::endl;
  perf_table2.changeMode("crit");
  std::cout << perf_table2 << std::endl;
  std::cout << "\n";

  std::cout << "Mode has changed to alt" << std::endl;
  perf_table2.changeMode("alt");
  std::cout << perf_table2 << std::endl;
  std::cout << "\n";
}
