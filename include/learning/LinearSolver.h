#ifndef LINEARSOLVER_H
#define LINEARSOLVER_H

#include "../app.h"
#include "ortools/linear_solver/linear_solver.h"
#include "spdlog/spdlog.h"

class LinearSolver {
public:
  LinearSolver(Config &conf);

private:
  Config conf;
};

#endif