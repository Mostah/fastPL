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
   * @param ap AlternativesPerformance objet that represents the dataset of the
   * problem
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

  /** updateWeightsAndLambda implements the metaheuristic that updates the
   * weights and the majority threshold given the state of the profiles in the
   * model.
   *
   * @param model MRSortModel to update
   */
  void updateWeightsAndLambda(MRSortModel &model);

  /** computexMatrixX Computes linear constraint matrix for x variables. X
   * Constraint matrix are of dimension (n_prof - 1, n_alt, n_crit). In
   * the first dimension, we have the n_prof - 1 profiles of the problem. Inside
   * profile h, we have a row for all alternatives, empty if not classified in
   * profile h. The representation of an alternative a classified in profile h
   * represents the presence or absence of each criterion in the constraint
   * (satisfied as a_i >= b_h-1_i for i criterion).
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
   * profile h, we have a row for all alternatives, empty if not classified in
   * profile h. The representation of an alternative a classified in profile h
   * represents the presence or absence of each criterion in the constraint
   * (satisfied as a_i >= b_h_i for i criterion).
   *
   * @param model MRSortModel use to compute the y matrix
   *
   * @return y_matrix matrix recapitulating the constraints to add to the linear
   * problem for the y variables
   */
  std::vector<std::vector<std::vector<bool>>>
  computeYMatrix(MRSortModel &model);

  /** modelCheck Checks if the profile is suited to be updated with this weight
   * updater: it checks if the criterion in the profile appears in the same
   * order as in the dataset.
   *
   * @param model MRSortModel to check
   *
   * @return is_suited
   */
  bool modelCheck(MRSortModel &model);

private:
  LinearSolver solver;
  AlternativesPerformance &ap;
  Config &conf;
};

#endif