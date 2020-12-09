#include <iostream>
#include <string>
#include <vector>

class Criterion {
public:
    Criterion(std::string id);
    Criterion(std::string id, std::string name, bool disabled, int direction, float weight);
    Criterion(const Criterion &crit); // Constructeur de copie
    ~Criterion();
    friend std::ostream & operator <<( std::ostream & out, const Criterion & crit );

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
     * getDisabled getter of disabled parameter
     *
     * @return disabled
     */
    bool getDisabled() const;

    /**
     * setDisabled setter of disabled parameter
     *
     * @param disabled
     */
    void setDisabled(bool disabled);

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
    /**
     * Kwargs:
     * id (str): Identifier of the criterion
     * name (str): A friendly name for the criterion
     * disabled (bool): Whether or not this criterion is disabled
     * direction (integer): Equal to -1 if criterion is to minimize,
     * 1 if the criterion is to maximize
     * weight (float): Deprecated
     * thresholds (list): List of threshold associated to the criterion
    */
    std::string id_;
    std::string name_;
    bool disabled_;
    int direction_;
    float weight_;
};