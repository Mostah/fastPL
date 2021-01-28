#include "../../include/learning/WeightUpdater.h"
#include "../../include/learning/LinearSolver.h"

WeightUpdater::WeightUpdater(AlternativesPerformance &ap, Config &conf)
    : ap(ap), conf(conf), solver(ap, conf) {}

WeightUpdater::WeightUpdater(const WeightUpdater &wu)
    : solver(wu.solver), ap(wu.ap), conf(wu.conf) {}

WeightUpdater::~WeightUpdater() {}

void WeightUpdater::updateWeightsAndLambda(MRSortModel &model) {
  auto matrix_x = this->computeXMatrix(model);
  auto matrix_y = this->computeYMatrix(model);
  std::pair<float, std::vector<float>> res = solver.solve(matrix_x, matrix_y);

  // update lambda
  model.lambda = res.first;
  // update all weights in order
  for (int i = 0; i < model.criteria.getCriterionVect().size(); i++) {
    model.criteria[i].setWeight(res.second[i]);
  }
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
          std::cout << alt[j].getValue() << " " << profs_pt[h][j].getValue();
          y_h_alt.push_back(alt[j].getValue() >= profs_pt[h][j].getValue());
        }
      }
      y_h.push_back(y_h_alt);
    }
    y_matrix.push_back(y_h);
  }
  return y_matrix;
}
