#ifndef PROFILEINITIALIZER_H
#define PROFILEINITIALIZER_H

/**
 * @file ProfileInitializer.h
 * @brief Profile initializer heuristic.
 *
 */

#include "../app.h"
#include "../types/AlternativesPerformance.h"
#include "../types/Categories.h"
#include "../types/MRSortModel.h"
#include "../types/Perf.h"
#include "../types/Profiles.h"

/** @class ProfileInitializer ProfileInitializer.h
 *  @brief Profile initializer heuristic.
 *
 * This class implements the profile initializer heuristic as described in the
 * thesis. This algorithm is initialized with the dataset and can then be used
 * to transform a model into an "initialized" one.
 *
 * A complete description of the heuristic can be found in @subpage
 * init_profiles.
 */

class ProfileInitializer {
public:
  /**
   * ProfileInitializer standard constructor.
   *
   * @param altPerfs AlternativesPerformance objet that represents our dataset
   * @param config config setup for logging
   *
   */
  ProfileInitializer(Config &config, AlternativesPerformance &altPerfs);

  /**
   * ProfileInitializer standard copy constructor.
   *
   * @param profInit ProfileInitializer object
   * @param config config setup for logging
   *
   */
  ProfileInitializer(const ProfileInitializer &profInit);

  /**
   * ProfileInitializer standard deconstructor
   *
   */
  ~ProfileInitializer();

  /**
   * Getter of the alternativePerformance class attribute
   *
   * @return AlternativesPerformance object
   */
  AlternativesPerformance getAlternativesPerformance() const;

  /**
   * Setter of the alternativePerformance class attribute
   *
   * @param newAltPerfs AlternativesPerformance objet that represents our
   * dataset
   */
  void setAlternativesPerformance(AlternativesPerformance &newAltPerfs);

  /**
   * Computes the frequency with which alternatives in the learning set are
   * assigned to each category
   *
   * @return vector of those frequency
   */
  std::vector<float> categoryFrequency();

  /**
   * Get ids of potential alternative candidates to initialize the profile
   * performance for one specific criterion and category
   *
   * @param crit Criterion object
   * @param cat Category object
   * @param nbCategories number of categories in our dataset
   *
   * @return vector of alternative ids
   */
  std::vector<Perf> getProfilePerformanceCandidates(const Criterion &crit,
                                                    const Category &cat,
                                                    const int nbCategories);

  /**
   * Compute the likelihood of choosing the performance of the alternative
   * identified by its altId as the performance value for profile delimiting
   * catAbove and catBelow for criterion crit.
   *
   * @param perfAlt Perf object of an alternative
   * @param crit Criterion object
   * @param catAbove Category object category above imaginary profile
   * performance (ie performance of altId)
   * @param catBelow Category object category below  imaginary profile
   * performance (ie performance of altId)
   * @param nbCategories number of categories in our dataset
   * @param catFrequency category frequency of our dataset
   * @param candidates potential candidates that were among our imaginary
   * profile performance (ie performance of altId)
   * @param delta articifial integer used to compare each candidates with the
   * imaginary profile performanc.
   *
   * @return probability of choosing alt_id for crit.
   */
  float weightedProbability(const Perf perfAlt, const Criterion &crit,
                            const Category &catAbove, const Category &catBelow,
                            const int nbCategories,
                            const std::vector<float> &catFrequency,
                            std::vector<Perf> &candidates, float delta = 0.001);

  /**
   * Initialize all of the profile performance values for Criterion crit
   *
   * @param crit Criterion object
   * @param categories Categories object
   * @param catFrequency category frequency of our dataset
   * @return initialized profile performances for Criterion crit
   */
  std::vector<Perf>
  initializeProfilePerformance(const Criterion &crit, Categories &categories,
                               const std::vector<float> &catFre);

  /**
   * Updates profile attribute from MRSortModel class with the a new profile
   * given by the metaheuristic.
   *
   * @param MRSortModel Mrsort model object
   */
  void initializeProfiles(MRSortModel &model);

private:
  Config &conf;
  AlternativesPerformance &altPerformance_;
};

#endif
