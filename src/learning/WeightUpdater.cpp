#include "../../include/learning/WeightUpdater.h"
#include "../../include/learning/LinearSolver.h"

#include <sstream>

WeightUpdater::WeightUpdater(AlternativesPerformance &ap, Config &conf)
    : ap(ap), conf(conf), solver(ap, conf) {}

WeightUpdater::WeightUpdater(const WeightUpdater &wu)
    : solver(wu.solver), ap(wu.ap), conf(wu.conf) {}

WeightUpdater::~WeightUpdater() {}

void WeightUpdater::updateWeightsAndLambda(MRSortModel &model) {

  if (!this->modelCheck(model)) {
    throw std::invalid_argument("Model's profile doesn't suite the alternative "
                                "performance of this WeightUpdater");
  }

  auto matrix_x = this->computeXMatrix(model);
  auto matrix_y = this->computeYMatrix(model);
  std::pair<float, std::vector<float>> res = solver.solve(matrix_x, matrix_y);

  std::ostringstream ss;
  ss << "Linear problem results - ";

  // update lambda
  model.lambda = res.first;
  ss << "Lambda: " << res.first;
  // update all weights in order
  model.criteria.setWeights(res.second);
  for (int i = 0; i < res.second.size(); i++) {
    ss << " - w" << i << ": " << model.criteria[i].getWeight();
  }
  conf.logger->info(ss.str());
}

std::vector<std::vector<std::vector<bool>>>
WeightUpdater::computeXMatrix(MRSortModel &model) {

  std::vector<std::vector<std::vector<bool>>> x_matrix;
  auto profs_pt = model.profiles.getPerformanceTable();
  auto alts_pt = ap.getPerformanceTable();
  auto alts_assign = ap.getAlternativesAssignments();
  // As the profiles are stored in descending order (h = 0 is the highest
  // profile, which is not the case in the theorical formulation), going in
  // reverse.
  for (int h = profs_pt.size() - 2; h >= 0; h--) {
    std::vector<std::vector<bool>> x_h;
    for (auto alt : alts_pt) {
      std::vector<bool> x_h_alt;
      // if alt is assigned to category h (otherwise, append empty vector)
      if (ap.getAlternativeAssignment(alt[0].getName()).getCategoryId() ==
          profs_pt[h][0].getName()) {
        for (int j = 0; j < alt.size(); j++) {
          // condition: aj >= bj_h-1
          // h+1 because going through the vector in reverse (descending order).
          x_h_alt.push_back(alt[j].getValue() >= profs_pt[h + 1][j].getValue());
        }
      }
      x_h.push_back(x_h_alt);
    }
    x_matrix.push_back(x_h);
  }
  return x_matrix;
}

std::vector<std::vector<std::vector<bool>>>
WeightUpdater::computeYMatrix(MRSortModel &model) {

  std::vector<std::vector<std::vector<bool>>> y_matrix;
  auto profs_pt = model.profiles.getPerformanceTable();
  auto alts_pt = ap.getPerformanceTable();
  auto alts_assign = ap.getAlternativesAssignments();

  // As the profiles are stored in descending order (h = 0 is the highest
  // profile, which is not the case in the theorical formulation), going in
  // reverse.
  for (int h = profs_pt.size() - 1; h > 0; h--) {
    std::vector<std::vector<bool>> y_h;
    for (auto alt : alts_pt) {
      std::vector<bool> y_h_alt;
      // if alt is assigned to category h (otherwise, append empty vector)
      if (ap.getAlternativeAssignment(alt[0].getName()).getCategoryId() ==
          profs_pt[h][0].getName()) {
        for (int j = 0; j < alt.size(); j++) {
          // condition: aj >= bj_h
          y_h_alt.push_back(alt[j].getValue() >= profs_pt[h][j].getValue());
        }
      }
      y_h.push_back(y_h_alt);
    }
    y_matrix.push_back(y_h);
  }
  return y_matrix;
}

bool WeightUpdater::modelCheck(MRSortModel &model) {
  if (model.profiles.getNumberCrit() != ap.getNumberCrit()) {
    return false;
  }
  // both are supposed to be in mode alt
  auto profs = model.profiles.getPerformanceTable();
  auto ap_pt = ap.getPerformanceTable();
  for (int i = 0; i < ap.getNumberCrit(); i++) {
    if (profs[0][i].getCrit() != ap_pt[0][i].getCrit()) {
      return false;
    }
  }
  return true;
}
