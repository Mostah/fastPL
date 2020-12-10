#ifndef CRITERIA_H
#define CRITERIA_H

#include <iostream>
#include <vector>
#include <Criterion>

class Criteria {
    public:
    /**
     * Criteria constructor with defined vector of criteria
     *
     * @param criteria_list Vector of criteria
     */
    Criteria(std::vector<Criterion> criteria_list);

    /**
     * Criteria constructor without vector
     *
     * @param nb_of_criteria Number of criteria
     * @param prefix Prefix to identify criteria
     */
    Criteria(int nb_of_criteria, std::string prefix);

    /**
     * Criteria constructor by copy
     *
     * @param crits Based criteria to copy
     */
    Criteria(const Criteria &crits);

    ~Criteria();
    friend std::ostream & operator <<( std::ostream & out, const Criteria & crits );

private:

    std::vector<Criterion> criteria_list_;
    int nb_of_criteria_;
    std::string prefix_;
};


#endif