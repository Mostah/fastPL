#ifndef CRITERION_H
#define CRITERION_H

#include <iostream>
#include <string>
#include <vector>

#include "AtomicMCDAObject.h"

class Criterion : public AtomicMCDAObject  {
public:

    /**
     * Criterion standard constructor
     *
     * @param id Identifier of the criterion
     * @param name Friendly name of the criterion
     * @param direction Equal to -1 if criterion is to minimize, 1 if the criterion is to maximize
     * @param weight Weight of the criterion
     */
    Criterion(std::string id, std::string name, int direction, float weight);

    /**
     * Criterion constructor without specification
     *
     * @param id Identifier of the criterion
     */
    Criterion(std::string id);

    /**
     * Criterion constructor by copy
     *
     * @param crit Based criterion to copy
     */
    Criterion(const Criterion &crit);

    ~Criterion();
    friend std::ostream & operator <<( std::ostream & out, const Criterion & crit );

    /**
     * generateDirection generator of random direction
     * can also be used as reinitialisation for direction
     * 
     */
    void generateDirection(unsigned long int seed = time(NULL));

    /**
     * generateWeight generator of random weight
     * can also be used as reinitialisation for weight
     * 
     */
    // void generateWeight(unsigned long int seed = std::random_device());
    void generateWeight(unsigned long int seed = time(NULL));

    /**
     * getId getter of id parameter
     *
     * @return id
     */
    std::string getId() const;

    /**
     * setId setter of id parameter
     *
     * @param id
     */
    void setId(std::string id);

    /**
     * getName getter of name parameter
     *
     * @return name
     */
    std::string getName() const;

    /**
     * setName setter of name parameter
     *
     * @param name
     */
    void setName(std::string name);

    /**
     * getDirection getter of direction parameter
     *
     * @return direction
     */
    int getDirection() const;

    /**
     * setDirection setter of direction parameter
     *
     * @param direction
     */
    void setDirection(int direction);


    /**
     * getWeight getter of weight parameter
     *
     * @return weight
     */
    float getWeight() const;

    /**
     * setWeight setter of weight parameter
     *
     * @param weight
     */
    void setWeight(float weight);


private:

    std::string id_;
    std::string name_;
    int direction_;
    float weight_;
};

#endif