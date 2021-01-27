#ifndef WEIGHTUPDATER_H
#define WEIGHTUPDATER_H

#include "../app.h"
#include "spdlog/spdlog.h"

#include "../types/AlternativesPerformance.h"
#include "../types/MRSortModel.h"
#include "LinearSolver.h"

class WeightUpdater {
public:
  /**
   * WeightUpdater standard constructor.
   *
   * @param ap AlternativesPerformance objet that represents our dataset
   * @param config config setup of the app
   */
  WeightUpdater(AlternativesPerformance &ap, Config &conf);

  /**
   * WeightUpdater constructor by copy.
   *
   * @param wu WeightUpdater objet to copy.
   */
  WeightUpdater(const WeightUpdater &wu);

  ~WeightUpdater();

  /** updateWeightsAndLambda implements the metaheuristic that update the
   * weights and the majority threshold given the state of the profiles in the
   * model.
   *
   * @param model MRSortModel to update
   */
  void updateWeightsAndLambda(MRSortModel &model);

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

  /** computexMatrixX Computes linear constraint matrix for x variables. X
   * Constraint matrix are of dimension (n_prof - 1, n_alt, n_crit). In
   * the first dimension, we have the n_prof - 1 profiles of the problem. Inside
   * profile h, we have a row for all alternatives that are classified in
   * profile h. The representation of an alternative a is the presence or
   * absence of each criterion in the constraint (satisfied as a_i >= b_h-1_i
   * for i criterion).
   *
   * @param model MRSortModel use to compute the x matrix
   *
   * @return x_matrix matrix recapitulating the constraints to add to the linear
   * problem for the x variables
   */
  std::vector<std::vector<std::vector<bool>>>
  computeXMatrix(MRSortModel &model);

  /** computexMatrixY Computes linear constraint matrix for y variables. Y
   * Constraint matrix are of dimension (n_prof - 1, n_alt, n_crit). In
   * the first dimension, we have the n_prof - 1 profiles of the problem. Inside
   * profile h, we have a row for all alternatives that are classified in
   * profile h. The representation of an alternative a is the presence or
   * absence of each criterion in the constraint (satisfied as a_i >= b_h_i for
   * i criterion).
   *
   * @param model MRSortModel use to compute the y matrix
   *
   * @return y_matrix matrix recapitulating the constraints to add to the linear
   * problem for the y variables
   */
  std::vector<std::vector<std::vector<bool>>>
  computeYMatrix(MRSortModel &model);

private:
  LinearSolver solver;
  AlternativesPerformance &ap;
  Config &conf;
};

#endif