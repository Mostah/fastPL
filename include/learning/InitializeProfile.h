#ifndef InitializeProfile_H
#define InitializeProfile_H

#include "../types/AlternativesPerformance.h"

class ProfileInitializer : public AtomicMCDAObject {
public:
  /**
   * Getter of the alternativePerformance class attribute
   *
   * @return AlternativesPerformance object
   */
  AlternativesPerformance getAlternativesPerformance() const;

  /**
   * Setter of the alternativePerformance class attribute
   *
   * @return null
   */
  void setAlternativesPerformance(AlternativesPerformance newAltPerfs);

  /**
   * Computes the frequency with which alternatives in the learning set are
   * assigned to each category
   *
   * @return vector of those frequency
   */
  std::vector<float> categoryFrequency(AlternativesPerformance newAltPerfs);

  /**
   * Computes the frequency with which alternatives in the learning set are
   * assigned to each category
   *
   * @return vector of those frequency
   */
  std::vector<float> categoryFrequency();

  /**
   * Get id of potential alternative candidates to initialize the profile
   * performance for one specific criterion and category
   *
   * @return vector of alternative ids
   */
  std::vector<std::string> getProfilePerformanceCandidates(Criterion crit,
                                                           Category cat);

  /**
   * Compute the likelyhood of chosing the performance of the alternative
   * identified by its altId as the performance value for profile delimting
   * catAbove and catBelow for criterion crit
   *
   * @return weighted probability
   */
  float weightedProbability(std::string altId, Criterion crit,
                            Category catAbove, Category catBelow);

  /**
   * Initialize all of the profile performance values for Criterion crit
   *
   * @return initialized profile performances for Criterion crit
   */
  std::vector<float> initializeProfilePerformance(Criterion crit);

  /**
   * Initialize all profiles
   * I would need to be able to create a PerformanceTable with mode="crit"
   * @return profile PerformanceTable
   */
  PerformanceTable initializeProfiles();

private:
  AlternativesPerformance altPerformance_;
};
#endif
