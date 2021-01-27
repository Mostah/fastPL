#include "../../include/learning/LinearSolver.h"
#include "../../include/types/AlternativesPerformance.h"
#include "../../include/types/Criteria.h"
#include "ortools/linear_solver/linear_solver.h"
#include "gtest/gtest.h"
#include <sstream>
#include <utility>

TEST(TestLinearSolver, TestInitializeSolver) {
  Criteria crits = Criteria(2);
  Config conf;
  AlternativesPerformance ap = AlternativesPerformance(4, crits);
  LinearSolver ls = LinearSolver(ap, conf);
  ls.initializeSolver();

  operations_research::MPSolver *solver = ls.getSolver();
  EXPECT_EQ(solver->NumVariables(), 19);
  EXPECT_EQ(solver->NumConstraints(), 1);
}

TEST(TestLinearSolver, TestUpdateConstraints) {
  // Will implement:
  // cst_x_b0_a0 : w0 + w1
  // cst_x_b0_a1 : w1
  // cst_x_b1_a2 : w0 + w1 + w2
  std::vector<std::vector<std::vector<bool>>> matrix_x{
      {{true, true, false}, {false, true, false}, {}},
      {{}, {}, {true, true, true}}};
  // cst_y_b0_a0 : w2
  // cst_y_b1_a0 : w1 + w2
  // cst_y_b1_a2 : w0 + w2
  std::vector<std::vector<std::vector<bool>>> matrix_y{
      {{false, false, true}, {}, {}},
      {{false, true, true}, {}, {true, false, true}}};
  Criteria crits = Criteria(3);
  Config conf;
  AlternativesPerformance ap = AlternativesPerformance(3, crits);
  LinearSolver ls = LinearSolver(ap, conf);

  ls.updateConstraints(matrix_x, matrix_y);

  operations_research::MPSolver *solver = ls.getSolver();
  EXPECT_EQ(solver->NumConstraints(), 7);
  auto weights = ls.getWeights();
  auto x_ap = ls.getXap();
  auto y_ap = ls.getYap();
  auto x_a = ls.getXa();
  auto y_a = ls.getYa();
  auto csts = solver->constraints();

  // sum_weights : w0 + w1 + w2
  auto sum_weights = csts[0];
  EXPECT_EQ(sum_weights->GetCoefficient(weights[0]), 1);
  EXPECT_EQ(sum_weights->GetCoefficient(weights[1]), 1);
  EXPECT_EQ(sum_weights->GetCoefficient(weights[2]), 1);

  // cst_x_b0_a0 : w0 + w1 - x0 + xp0
  auto cst_x_b0_a0 = csts[1];
  EXPECT_EQ(cst_x_b0_a0->GetCoefficient(weights[0]), 1);
  EXPECT_EQ(cst_x_b0_a0->GetCoefficient(weights[1]), 1);
  EXPECT_EQ(cst_x_b0_a0->GetCoefficient(weights[2]), 0);
  EXPECT_EQ(cst_x_b0_a0->GetCoefficient(x_a[0]), -1);
  EXPECT_EQ(cst_x_b0_a0->GetCoefficient(x_ap[0]), 1);

  // cst_x_b0_a1 : w1 - x1 + xp1
  auto cst_x_b0_a1 = csts[2];
  EXPECT_EQ(cst_x_b0_a1->GetCoefficient(weights[0]), 0);
  EXPECT_EQ(cst_x_b0_a1->GetCoefficient(weights[1]), 1);
  EXPECT_EQ(cst_x_b0_a1->GetCoefficient(weights[2]), 0);
  EXPECT_EQ(cst_x_b0_a1->GetCoefficient(x_a[1]), -1);
  EXPECT_EQ(cst_x_b0_a1->GetCoefficient(x_ap[1]), 1);

  // cst_x_b1_a2 : w0 + w1 + w2 - x2 + xp2
  auto cst_x_b1_a2 = csts[3];
  EXPECT_EQ(cst_x_b1_a2->GetCoefficient(weights[0]), 1);
  EXPECT_EQ(cst_x_b1_a2->GetCoefficient(weights[1]), 1);
  EXPECT_EQ(cst_x_b1_a2->GetCoefficient(weights[2]), 1);
  EXPECT_EQ(cst_x_b1_a2->GetCoefficient(x_a[2]), -1);
  EXPECT_EQ(cst_x_b1_a2->GetCoefficient(x_ap[2]), 1);

  // cst_y_b0_a1 : w2 + y0 - yp0
  auto cst_y_b0_a1 = csts[4];
  EXPECT_EQ(cst_y_b0_a1->GetCoefficient(weights[0]), 0);
  EXPECT_EQ(cst_y_b0_a1->GetCoefficient(weights[1]), 0);
  EXPECT_EQ(cst_y_b0_a1->GetCoefficient(weights[2]), 1);
  EXPECT_EQ(cst_y_b0_a1->GetCoefficient(y_a[0]), 1);
  EXPECT_EQ(cst_y_b0_a1->GetCoefficient(y_ap[0]), -1);

  // cst_y_b1_a0 : w1 + w2 + y0 - yp0
  auto cst_y_b1_a0 = csts[5];
  EXPECT_EQ(cst_y_b1_a0->GetCoefficient(weights[0]), 0);
  EXPECT_EQ(cst_y_b1_a0->GetCoefficient(weights[1]), 1);
  EXPECT_EQ(cst_y_b1_a0->GetCoefficient(weights[2]), 1);
  EXPECT_EQ(cst_y_b1_a0->GetCoefficient(y_a[0]), 1);
  EXPECT_EQ(cst_y_b1_a0->GetCoefficient(y_ap[0]), -1);

  // cst_y_b1_a2 : w0 + w2 + y2 - yp2
  auto cst_y_b1_a2 = csts[6];
  EXPECT_EQ(cst_y_b1_a2->GetCoefficient(weights[0]), 1);
  EXPECT_EQ(cst_y_b1_a2->GetCoefficient(weights[1]), 0);
  EXPECT_EQ(cst_y_b1_a2->GetCoefficient(weights[2]), 1);
  EXPECT_EQ(cst_y_b1_a2->GetCoefficient(y_a[2]), 1);
  EXPECT_EQ(cst_y_b1_a2->GetCoefficient(y_ap[2]), -1);
}