#include "../include/utils.h"

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec) {
  out << "[";
  for (std::size_t i = 0; i < vec.size(); i++) {
    out << vec[i] << (i == vec.size() - 1 ? "" : ",");
  }
  out << "]";
  return out;
}

template <typename T>
std::vector<T> subVector(std::vector<T> const &v, int m, int n) {
  auto first = v.begin() + m;
  auto last = v.begin() + n + 1;
  std::vector<T> vector(first, last);
  return vector;
}

// struct simple_walker : pugi::xml_tree_walker {
//   virtual bool for_each(pugi::xml_node &node) {
//     for (int i = 0; i < depth(); ++i)
//       std::cout << "  "; // indentation

//     std::cout << node.type() << ": name='" << node.name() << "', value='"
//               << node.value() << "'\n";

//     return true; // continue traversal
//   }
// };

inline bool fileExists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

inline float getRandomUniformNumber(bool changeSeed) {
  if (changeSeed) {
    srand(time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
  } else {
    srand(0);
  }
  return (float)(rand() % 1000) / 1000;
}

inline float getRandomUniformNumberBis(bool changeSeed) {
  if (changeSeed) {
    srand(time(NULL));
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    return (float)(rand() % 1000) / 2000 + 0.5;
  } else {
    return (float)(rand() % 1000) / 2000 + 0.5;
  }
}

inline int getRandomUniformInt(unsigned long int seed, int min, int max) {
  srand(seed);
  return min + rand() % (max - min);
}

inline float getRandomUniformFloat(unsigned long int seed, float min,
                                   float max) {
  srand(seed);
  return min + (((float)rand()) / (float)RAND_MAX) * (max - min);
}

inline std::vector<float> randomCriteriaLimits(int nbCategories,
                                               bool changeSeed) {
  std::vector<float> critLimits;
  for (int i = 0; i < nbCategories; i++) {
    critLimits.push_back(getRandomUniformNumber(changeSeed));
  }
  sort(critLimits.begin(), critLimits.end());
  return critLimits;
}