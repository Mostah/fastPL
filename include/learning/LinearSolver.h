#ifndef LINEARSOLVER_H
#define LINEARSOLVER_H

/**
 * @file LinearSolver.h
 * @brief External linear solver wrapper.
 *
 */

#include "../app.h"
#include "../types/AlternativesPerformance.h"
#include "ortools/linear_solver/linear_solver.h"
#include "spdlog/spdlog.h"

#include <string>

/** @class LinearSolver LinearSolver.h
 *  @brief External linear solver wrapper
 *
 * The linear solver class is a wrapper that makes an abstraction between the
 * weight updater and the resolution of the linear problem. Currently this
 * wrapper uses ortools interface and only the ortools solver (GLOP) is
 * implemented. Other solver could be used by following the instruction on how
 * to install them for ortools and changing the initialisation of the solver to
 * use the right one, however the implementation of the resolution shouldn't
 * have to be changed.
 *
 * The abstraction was made by receiving the constraint matrices according to
 * the equations described in the thesis and returning the solution found for
 * the new weights and lambda.
 *
 * Link to ortools (google) : https://github.com/google/or-tools
 */

class LinearSolver {
public:
  /**
   * LinearSolver standard constructor.
   *
   * @param ap AlternativesPerformance objet that represents the dataset of the
   * problem
   * @param config config setup from the app
   * @param solver solver type to use. Currently only "GLOP" is implemented
   */
  LinearSolver(AlternativesPerformance &ap, Config &conf,
               float delta = 0.000001, std::string solver = "GLOP");

  ~LinearSolver();

  /** initializeSolver Initialise the solver given the alternative performance
   * (dataset): add variables and constraints that are not changing given a
   * particular model.
   *
   */
  void initializeSolver();

  /** updateConstraints reset the previous constraints and add the new ones
   * given by the matrixes
   *
   * @param x_matrix matrix recapitulating the constraints to add to the linear
   * problem for the x variables
   * @param y_matrix matrix recapitulating the constraints to add to the linear
   * problem for the y variables
   *
   */
  void updateConstraints(std::vector<std::vector<std::vector<bool>>> x_matrix,
                         std::vector<std::vector<std::vector<bool>>> y_matrix);

  /** solve Solve the linear problem given the constraint matrix.
   *
   * @param x_matrix matrix recapitulating the constraints to add to the linear
   * problem for the x variables
   * @param y_matrix matrix recapitulating the constraints to add to the linear
   * problem for the y variables
   *
   * @return results contained in a pair of (lambda, vector of weights)
   */
  std::pair<float, std::vector<float>>
  solve(std::vector<std::vector<std::vector<bool>>> x_matrix,
        std::vector<std::vector<std::vector<bool>>> y_matrix);

  /**
   * getAlternativesPerformance getter of the alternative performance
   *
   * @return ap
   */
  AlternativesPerformance &getAlternativesPerformance() const;

  /**
   * getSolver getter of the solver
   *
   * @return solver
   */
  operations_research::MPSolver *getSolver() const;

  /**
   * getWeights getter of the weights
   *
   * @return weights
   */
  std::vector<operations_research::MPVariable *> getWeights() const;

  /**
   * getXa getter of the x_a variables
   *
   * @return x_a
   */
  std::vector<operations_research::MPVariable *> getXa() const;

  /**
   * getXap getter of the x_ap variables
   *
   * @return x_ap
   */
  std::vector<operations_research::MPVariable *> getXap() const;

  /**
   * getYa getter of the y_a variables
   *
   * @return y_a
   */
  std::vector<operations_research::MPVariable *> getYa() const;

  /**
   * getYap getter of the y_ap variables
   *
   * @return y_ap
   */
  std::vector<operations_research::MPVariable *> getYap() const;

  /**
   * getConf getter of the conf
   *
   * @return conf
   */
  Config getConf() const;

private:
  AlternativesPerformance &ap;
  operations_research::MPSolver *solver;
  std::string solver_name;
  Config &conf;

  // value use to transform strict inequalities into non-strict ones
  float delta;

  // variables
  std::vector<operations_research::MPVariable *> x_a;
  std::vector<operations_research::MPVariable *> x_ap;
  std::vector<operations_research::MPVariable *> y_a;
  std::vector<operations_research::MPVariable *> y_ap;
  std::vector<operations_research::MPVariable *> weights;
  operations_research::MPVariable *lambda;

  // constraints
  std::vector<operations_research::MPConstraint *> x_constraints;
  std::vector<operations_research::MPConstraint *> y_constraints;
  std::vector<operations_research::MPConstraint *> weights_constraint;
};

#endif