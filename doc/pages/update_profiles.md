@page update_profiles Profiles update

# Profiles update algorithm

## I - Context

Profile Updater algorithm is responsible for updating the profiles given the fixed weight and lambda. It will evaluate each move of each profile on each criterion to see what moves have the best impact on the alternatives assignments.

## II - ProfileUpdater

### Attributes

* **Epsilon** - Float (small value ~10-4) ensuring that the classification relationship is maintained between profile and alternatives (no profile will have the same value as an alt)
* **AlternativesPerformance** - dataset of the global problem
* **Config** - Global config of the application

### Description

After initializing the profiles, computing the weights and the lambda of a model, the last step is to update the profile to match as much as possible the decision maker's criteria.
The profile updater algorithm functions as below:

    1. For each profile:
        A. For each criterion:
            i. Compute the possible positions for the profile and evaluate the repercution of this move on the classification 
            ii. Select the position that maximizes the result of the move on the alternative assignments 
            iii. Move the profile and update the alternative assignments *(updateTables)*

Or, in terms of functions:

    1. *optimizeProfile* performs the algorithm above for one profile
        A. *optimize performs* optimizeProfile on each profile
            i. *computeBelowDisirability, computeAboveDesirability* 
            ii. *chooseMaxDesirability* 
            iii. *updateTables* 