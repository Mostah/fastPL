#ifndef InitializeProfile_H
#define InitializeProfile_H

#include "../app.h"
#include "../types/AlternativesPerformance.h"
#include "../types/Categories.h"
#include "../types/Profiles.h"

class ProfileInitializer {
public:
  /**
   * ProfileInitializer standard constructor
   *
   * @return ProfileInitializer object
   */
  ProfileInitializer(Config &config, AlternativesPerformance &altPerfs);

  /**
   * ProfileInitializer copy constructor
   *
   * @return ProfileInitializer object
   */
  ProfileInitializer(Config &config, const ProfileInitializer &profInit);

  /**
   * ProfileInitializer standard deconstructor
   *
   */
  ~ProfileInitializer(){};

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
  void setAlternativesPerformance(AlternativesPerformance &newAltPerfs);

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
  std::vector<std::string>
  getProfilePerformanceCandidates(const Criterion &crit, const Category &cat,
                                  const int nbCategories);

  /**
   * Compute the likelihood of choosing the performance of the alternative
   * identified by its altId as the performance value for profile delimiting
   * catAbove and catBelow for criterion crit.
   *
   * @return weighted probability
   */
  float weightedProbability(const std::string altId, const Criterion &crit,
                            const Category &catAbove, const Category &catBelow,
                            const int nbCategories,
                            const std::vector<float> &catFrequency,
                            std::vector<std::string> candidates,
                            float delta = 0.001);

  /**
   * Initialize all of the profile performance values for Criterion crit
   *
   * @return initialized profile performances for Criterion crit
   */
  Performance initializeProfilePerformance(const Criterion &crit,
                                           Categories &categories,
                                           const std::vector<float> &catFre);

  /**
   * Initialize all profiles
   * @return profile Profile in "crit" mode
   */
  Profiles initializeProfiles(Categories &categories);

private:
  Config &conf;
  AlternativesPerformance altPerformance_;
};

#endif
