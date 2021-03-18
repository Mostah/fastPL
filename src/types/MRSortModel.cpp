#include "../../include/types/MRSortModel.h"
#include "../../include/types/Category.h"
#include "../../include/types/Criteria.h"
#include "../../include/types/PerformanceTable.h"
#include "../../include/utils.h"

#include <cstdlib>
#include <map>
#include <string>

Categories default_cats;

MRSortModel::MRSortModel(Criteria &crits, Profiles &profs, Categories &cats,
                         float lbd, std::string id, float score)
    : criteria(crits), profiles(profs), categories(cats) {
  if (profiles.getPerformanceTable().size() !=
      cats.getIdCategories().size() - 1) {
    throw std::invalid_argument(
        "For n categories we must have n - 1 profiles.");
  }
  lambda = lbd;
  id_ = id;
  score_ = score;
}

MRSortModel::MRSortModel(int n_cat, int n_crit, std::string id, float score)
    : criteria(n_crit), profiles(n_cat - 1, criteria, "crit", "prof"),
      categories(default_cats) {
  if (n_cat < 2) {
    throw std::invalid_argument(
        "The number of categories (n_cat) must be >= 2");
  }
  id_ = id;
  score_ = score;
  categories = Categories(n_cat);

  lambda = getRandomUniformFloat(time(NULL), 0.5, 1);
  criteria.generateRandomCriteriaWeights();
  profiles.generateRandomPerfValues();
}

MRSortModel::MRSortModel(const MRSortModel &mrsort)
    : criteria(mrsort.criteria), profiles(mrsort.profiles),
      categories(mrsort.categories) {
  lambda = mrsort.lambda;
  accuracy = mrsort.accuracy;
  id_ = mrsort.id_;
}

MRSortModel::categoryAssignment(std::vector<Perf> &alt,
                                std::vector<std::vector<Perf>> &profiles_pt) {
  bool assigned = false;
  Category cat_assignment;
  // For all alt, looping over all profiles in descending order
  for (int h = (profiles_pt.size() - 1); h >= 0; h--) {
    // compute the concordance value:
    float c = computeConcordance(profiles_pt[h], alt);
    // if the value of the concordance is greater than the threshold, assign
    // the category h to the alt.
    // As we are going in descending order, the category assigned is the
    // highest possible, guaranteeing that we have c > lamdba for profile h
    // and c < lambda for profile h+1.
    if (c >= lambda) {
      cat_assignment = categories.getCategoryOfRank(h + 1);
      assigned = true;
      break;
    }
  }
  // if for all profile, the concordance is below the threshold, assigned the
  // alt to the lowest category
  if (!assigned) {
    cat_assignment = categories.getCategoryOfRank(0);
  }
  return cat_assignment;
}

AlternativesPerformance MRSortModel::categoryAssignments(PerformanceTable &pt) {
  if (pt.getMode() != "alt") {
    throw std::invalid_argument(
        "Performance table set in wrong mode, should be alt.");
  }
  std::unordered_map<std::string, Category> cat_assignments;
  std::vector<std::vector<Perf>> profiles_pt = profiles.getPerformanceTable();
  // Looping over all alternatives
  for (std::vector<Perf> alt : pt.getPerformanceTable()) {
    cat_assignments[alt[0].getName()] = categoryAssignment(alt, profiles_pt);
  }
  return AlternativesPerformance(pt, cat_assignments);
}

float MRSortModel::computeConcordance(std::vector<Perf> &prof,
                                      std::vector<Perf> &alt) {
  float c = 0;
  for (Perf perf_j : alt) {
    for (Perf prof_i : prof) {
      // If the value of the alt on criterion j is greater than the one of
      // the profile h, add the weight of the criterion j to the concordance
      // value.
      if ((prof_i.getCrit() == perf_j.getCrit()) &&
          (perf_j.getValue() > prof_i.getValue())) {
        c = c + criteria[perf_j.getCrit()].getWeight();
      }
    }
  }
  return c;
}

std::unordered_map<std::string, std::unordered_map<std::string, float>>
MRSortModel::computeConcordanceTable(PerformanceTable &pt) {
  if (pt.getMode() != "alt") {
    throw std::invalid_argument(
        "Performance table set in wrong mode, should be alt.");
  }
  std::unordered_map<std::string, std::unordered_map<std::string, float>> ct;
  std::vector<std::vector<Perf>> profiles_pt = profiles.getPerformanceTable();
  // Looping over all profiles
  for (int h = 0; h < profiles_pt.size(); h++) {
    std::unordered_map<std::string, float> prof_concordances;
    // Looping over all alternatives
    for (std::vector<Perf> alt : pt.getPerformanceTable()) {
      prof_concordances[alt[0].getName()] =
          computeConcordance(profiles_pt[h], alt);
    }
    ct[profiles_pt[h][0].getName()] = prof_concordances;
  }
  return ct;
}

MRSortModel::~MRSortModel() {}

std::string MRSortModel::getId() const { return id_; }

float MRSortModel::getScore() const { return score_; }

void MRSortModel::setScore(float score) { score_ = score; }

std::ostream &operator<<(std::ostream &out, const MRSortModel &mrsort) {
  out << "Model( id : " << mrsort.id_ << std::endl
      << ", lambda : " << mrsort.lambda << std::endl
      << ", crit : " << mrsort.criteria << std::endl
      << ", profiles : " << mrsort.profiles << ")";
  return out;
}