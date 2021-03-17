@page weight_updater Lambda and Weight updater

# Lambda and Weight updater algorithm

## I - Context

The weight updater algorithm is responsible for updating the weight associated to each Criterion and the lambda (majority threshold) of the problem.
The weight updater algorithm is divided in two main components (C++ classes): the LinearSolver and the WeightUpdater.

## II - WeightUpdater

The weightUpdater is responsible of computing the constraint matrix and pass it to the linear solver before updating the MRSortModel with the new weights and lambda.

### Attributes

* Config - Global config of the application
* LinearSolver - abstraction between the algorithm and the way we solve the linear problem
* AlternativesPerformance - dataset of the global problem

### Matrix constraint representation

The datastructre of the matrix constraint is a 3-dimension boolean tensor. The first dimension represent a profile, se second an alternative and the third a criteria. Thus a constrain matrix is a tensor of dimension : n_prof x n_alt x n_crit.

For a specific profile and alternative, the booleans denote the presence or absence of the criterion in the constraint. 

For example, given {w0, ..., w4} weights of 5 different criterion:
```
w0 + w3 + w4
```
is represented by
```
[true, false, false, true, true]
```

## III - LinearSolver

The linear solver makes an interface between our algorithm and the way we solve the linear problem. Therefore, if at some point the way of solving the linear problem is changed (changing the library for example), no modification in the WeightUpdater class will be needed.

The linear solver takes as input the constraint matrix and the variables of the problem to solve, instantiates a model solver and computes the new weights and lambda.

### Attributes

Delta: small value to transform strict equalities into non strict ones - default = 0.0001.
AlternativesPerformance - dataset of the global problem
Config - Global config of the application


### Definition of the Linear Problem

#### Variables

```
w_j in [0,1] for j in {1, …, n_crit}
lambda in [0.5,1]
x_a in [0,1] for a in {1, …, n_alt}
y_a in [0,1] for a in {1, …, n_alt}
xp_a in [0,1] for a in {1, …, n_alt}
yp_a in [0,1] for a in {1, …, n_alt}
```

#### Constraints

```
sum(w_j for j in {1, …, n_crit}) = 1
sum(w_j(a_i, b_h-1) if a_i>=bi_h-1) - x_a + xp_a = lambda, for a assigned to C_h, for h in [1, n_prof]
sum(w_j(a_i, b_h) if a_i>=bi_h) - y_a + yp_a = lambda - delta, for a assigned to C_h, for h in [0, n_prof-1]
```

#### Objective function

min(sum(x_ap+y_ap, for a in {0,…,n_alt}))