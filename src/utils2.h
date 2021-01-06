#ifndef UTILS2_H
#define UTILS2_H

#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include <vector>

/**
 * Random number generator between 0 and 1
 *
 * @return random  number
 */
inline float getRandomUniformNumber(bool changeSeed = 1) {
  if (changeSeed) {
    srand(time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
  } else {
    srand(0);
  }
  return (float)(rand() % 1000) / 1000;
}

/**
 * Random number generator between 1/2 and 1
 *
 * @return random  number
 */
inline float getRandomUniformNumberBis(bool changeSeed = 1) {
  if (changeSeed) {
    srand(time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    return (float)(rand() % 1000) / 2000 + 0.5;
  } else {
    srand(1);
    return (float)(rand() % 1000) / 2000 + 0.5;
  }
}

inline std::vector<float> randomCriteriaLimits(int nbCategories) {
  std::vector<float> critLimits;
  for (int i = 0; i < nbCategories; i++) {
    critLimits.push_back(getRandomUniformNumber());
  }
  sort(critLimits.begin(), critLimits.end());
  return critLimits;
}

#endif