#include "../../include/learning/LinearSolver.h"
#include "../../include/app.h"
#include "../../include/types/AlternativesPerformance.h"
#include "ortools/linear_solver/linear_solver.h"

#include <sstream>

LinearSolver::LinearSolver(AlternativesPerformance &ap, Config &conf,
                           float delta, std::string solver_name)
    : ap(ap), conf(conf) {
  solver = operations_research::MPSolver::CreateSolver(solver_name);
  solver_name = solver_name;
  delta = delta;
}

LinearSolver::~LinearSolver() { delete solver; }

AlternativesPerformance &LinearSolver::getAlternativesPerformance() const {
  return ap;
}

operations_research::MPSolver *LinearSolver::getSolver() const {
  return solver;
}

Config LinearSolver::getConf() const { return conf; }

std::vector<operations_research::MPVariable *>
LinearSolver::getWeights() const {
  return weights;
}

std::vector<operations_research::MPVariable *> LinearSolver::getXa() const {
  return x_a;
}

std::vector<operations_research::MPVariable *> LinearSolver::getXap() const {
  return x_ap;
}

std::vector<operations_research::MPVariable *> LinearSolver::getYa() const {
  return y_a;
}

std::vector<operations_research::MPVariable *> LinearSolver::getYap() const {
  return y_ap;
}

void LinearSolver::initializeSolver() {

  // weight variables
  for (int i = 0; i < ap.getNumberCrit(); i++) {
    weights.push_back(solver->MakeNumVar(0., 1.0, "w" + std::to_string(i)));
  }

  // x, x' (xp), y, y' (yp) variables
  for (int i = 0; i < ap.getNumberAlt(); i++) {
    x_a.push_back(solver->MakeNumVar(0., 1.0, "x" + std::to_string(i)));
    x_ap.push_back(solver->MakeNumVar(0., 1.0, "xp" + std::to_string(i)));
    y_a.push_back(solver->MakeNumVar(0., 1.0, "y" + std::to_string(i)));
    y_ap.push_back(solver->MakeNumVar(0., 1.0, "yp" + std::to_string(i)));
  }

  // lambda variable
  lambda = solver->MakeNumVar(0.5, 1.0, "lambda");

  // sum weight equal 1 constraint
  weights_constraint =
      solver->MakeRowConstraint(1 - delta, 1, "weight_constraint");
  for (operations_research::MPVariable *w_i : weights) {
    weights_constraint->SetCoefficient(w_i, 1);
  }

  // objective function, minimize sum(xp + yp)
  operations_research::MPObjective *const objective =
      solver->MutableObjective();
  for (operations_research::MPVariable *xp : x_ap) {
    objective->SetCoefficient(xp, 1);
  }
  for (operations_research::MPVariable *yp : y_ap) {
    objective->SetCoefficient(yp, 1);
  }
  objective->SetMinimization();
}

void LinearSolver::updateConstraints(
    std::vector<std::vector<std::vector<bool>>> x_matrix,
    std::vector<std::vector<std::vector<bool>>> y_matrix) {

  // reset constraints vectors
  x_constraints.clear();
  y_constraints.clear();

  // reset variables vectors
  x_a.clear();
  x_ap.clear();
  y_a.clear();
  y_ap.clear();
  weights.clear();

  // reset previous solver
  solver->Clear();

  // re-initialise solver with variable and default constraing
  this->initializeSolver();

  // add new constraints given the matrixs

  // Starting with x constraints
  // for all profiles
  for (int h = 0; h < x_matrix.size(); h++) {
    // for all alternative assigned to category h
    for (int alt = 0; alt < x_matrix[h].size(); alt++) {
      // if alt is empty the alt was not assigned to this category (h)
      if (!x_matrix[h][alt].empty()) {
        // create constraint with name cst_x_b2_a6
        operations_research::MPConstraint *cst = solver->MakeRowConstraint(
            0 - delta, 0 + delta,
            "cst_x_b" + std::to_string(h) + "_a" + std::to_string(alt));
        // +lambda
        cst->SetCoefficient(lambda, -1);
        // -x_a
        // +x_ap
        cst->SetCoefficient(x_a[alt], -1);
        cst->SetCoefficient(x_ap[alt], 1);
        // +sum(w_j(a_i, b_h-1) if a_i>=bi_h-1)
        for (int crit = 0; crit < x_matrix[h][alt].size(); crit++) {
          if (x_matrix[h][alt][crit]) {
            cst->SetCoefficient(weights[crit], 1);
          }
        }
        x_constraints.push_back(cst);
      }
    }
  }

  // Same for y constraints
  // for all profiles
  for (int h = 0; h < y_matrix.size(); h++) {
    // for all alternative assigned to category h
    for (int alt = 0; alt < y_matrix[h].size(); alt++) {
      // if alt is empty the alt was not assigned to this category (h)
      if (!y_matrix[h][alt].empty()) {
        // create constraint with name cst_y_b2_a6 for ex
        operations_research::MPConstraint *cst = solver->MakeRowConstraint(
            0 - delta, 0 + delta,
            "cst_y_h" + std::to_string(h) + "_a" + std::to_string(alt));
        // +lambda
        cst->SetCoefficient(lambda, -1);
        // y_a
        // -x_ap
        cst->SetCoefficient(y_a[alt], 1);
        cst->SetCoefficient(y_ap[alt], -1);
        // +sum(w_j(a_i, b_h-1) if a_i>=bi_h-1)
        for (int crit = 0; crit < y_matrix[h][alt].size(); crit++) {
          if (y_matrix[h][alt][crit]) {
            cst->SetCoefficient(weights[crit], 1);
          }
        }
        y_constraints.push_back(cst);
      }
    }
  }
}