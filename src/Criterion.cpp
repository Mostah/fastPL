#include "Criterion.h"
#include <iostream>
#include <string>


Criterion::Criterion(std::string id){
    id_ = id;
    name_ = "";
    disabled_ = false;
    direction_ = 1;
    weight_ = 0;
}

Criterion::Criterion(std::string id, std::string name, bool disabled, int direction, float weight){
    id_ = id;
    name_ = name;
    disabled_ = disabled;
    direction_ = direction;
    weight_ = weight;
}

Criterion::Criterion(const Criterion &crit){ 
    id_ = crit.getId();
    name_ = crit.getName();
    disabled_ = crit.getDisabled();
    direction_ = crit.getDirection();
    weight_ = crit.getWeight();
}

std::string Criterion::getId() const { return id_; }

void Criterion::setId(std::string id) { id_ = id; }

std::string Criterion::getName() const { return name_; }

void Criterion::setName(std::string name) { name_ = name; }

bool Criterion::getDisabled() const { return disabled_; }

void Criterion::setDisabled(bool disabled) { disabled_ = disabled; }

int Criterion::getDirection() const { return direction_; }

void Criterion::setDirection(int direction) { direction_ = direction; }

float Criterion::getWeight() const { return weight_; }

void Criterion::setWeight(float weight) { weight_ = weight; }



std::ostream & operator <<( std::ostream & out, const Criterion & crit ){
    std::string dir = "";
    if (crit.direction_ == 1)
    {
        dir = "+";
    }
    else
    {
        dir = "-";
    }
    out << "Criterion(" << crit.id_ << ":" << dir  << ")";
    return out;
}

Criterion::~Criterion(){}