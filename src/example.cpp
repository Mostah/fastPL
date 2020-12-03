#include "example.h"
#include <iostream>

// Définition des constructeurs, destructeurs et opérateurs de Nombre

Nombre::Nombre(){
    premier_ = new Chiffre(0);
}

Nombre::Nombre(unsigned long n){
    premier_ = new Chiffre(n) ;
}

// Constructeur de copie
Nombre::Nombre(const Nombre & nb){
    premier_ = new Chiffre(*nb.premier_);
}

// Destructeur de Nombre
Nombre::~Nombre(){
    delete premier_;
}

// Opérateur d'affichage
std::ostream & operator <<( std::ostream & out, const Nombre & n ){
    if (n.premier_ != nullptr){
        return n.premier_->print(out);
    }
    return out;
}

// Opérateur de lecture
std::istream & operator >>( std::istream & in, Nombre & n ){
    // in est le flux d'entrée
    Nombre::Chiffre* cfPasse = nullptr; // initialisation du chiffre passé 
    while( in.good() ) {
        int c{ in.get() };
        if( std::isdigit( c )) {
            unsigned int d{ static_cast<unsigned int>( c - '0' )};
            // d contient le chiffre entre 0 et 9 qui vient d'être lu
            Nombre::Chiffre* cfLu = new Nombre::Chiffre(d);   // le chiffre qui vient d'être lu
            cfLu->suivant_ = cfPasse;  // chiffre de poids supérieur
            cfPasse = cfLu;       
        }
        else {
            n.premier_ = cfPasse;
            break;
        }
    }
  return in;
}

void Nombre::swap(Nombre & nb){
    using std::swap;
    swap(premier_, nb.premier_);
}

// Opérateur d'affectation
Nombre & Nombre::operator =(const Nombre & nb){
    Nombre tmp{nb};
    swap(tmp);
    return *this;
}

// Opérateur += pour Nombre
Nombre & Nombre::operator +=(unsigned int i){
    if (premier_ != nullptr){
    *premier_ += i;
    }
    return *this;
}

// Opérateur *= pour Nombre
Nombre & Nombre::operator *=(unsigned int i){
    if (premier_ != nullptr){
    *premier_ *= i;
    }
    return *this;
}

// Opérateur + pour Nombre
Nombre operator +(const Nombre & nb, unsigned int i){
    Nombre tmp = nb;
    tmp += i;   // on utilise l'opérateur += créé au dessus
    return tmp;
}

// Opérateur * pour Nombre
Nombre operator *(const Nombre & nb, unsigned int i){
    Nombre tmp = nb;
    tmp *= i;   // on utilise l'opérateur *= créé au dessus
    return tmp;
}

// Définition des constructeurs, destructeurs et opérateurs de Chiffre

Nombre::Chiffre::Chiffre(unsigned long n){
    if (n / 10 == 0){  
        chiffre_ = n;
        suivant_ = nullptr;
    }
    else {
        chiffre_ = n%10;
        suivant_ = new Chiffre(n/10);
    }
}

// Constructeur de copie
Nombre::Chiffre::Chiffre(const Chiffre & cf){
    chiffre_ = cf.chiffre_;
    if (cf.suivant_ != nullptr){
        suivant_ = new Chiffre(*cf.suivant_);
    }
}

// Destructeur de Chiffre
Nombre::Chiffre::~Chiffre(){
    if (suivant_ != nullptr){
        delete suivant_;
    }
}

// Opérateur d'affichage (print) pour chiffre
std::ostream & Nombre::Chiffre::print( std::ostream & out ){
    if (suivant_ != nullptr){
        suivant_->print(out);
    }
    out << chiffre_;
    return out;
}

void Nombre::Chiffre::swap(Chiffre & cf){
    using std::swap;
    swap(chiffre_, cf.chiffre_);
    swap(suivant_, cf.suivant_);
}

// Opérateur d'affectation
Nombre::Chiffre & Nombre::Chiffre::operator =(const Chiffre & cf){
    Chiffre tmp{cf};
    swap(tmp);
    return *this;
}

// Opérateur += pour Chiffre
Nombre::Chiffre & Nombre::Chiffre::operator +=(unsigned int i){
    unsigned int tmp = chiffre_ + i;

    chiffre_ = tmp % 10;

    if (tmp / 10 != 0){  
        if (suivant_ != nullptr){
            *suivant_ += (tmp / 10);
        }
        else{
            suivant_ = new Chiffre(tmp / 10);
        }
    }
    return *this;
}

// Opérateur *= pour Chiffre
Nombre::Chiffre & Nombre::Chiffre::operator *=(unsigned int i){
    unsigned int tmp = chiffre_ * i;

    chiffre_ = tmp % 10;

    if (suivant_ != nullptr){  
            *suivant_ *= i;
            *suivant_ += tmp / 10; // correction - on ajoute la retenue!
        }
    else if (tmp / 10 != 0){
        suivant_ = new Chiffre(tmp / 10);
    }
    return *this;
}

// Opérateur + pour Chiffre
Nombre::Chiffre operator +(const Nombre::Chiffre & cf, unsigned int i){
    Nombre::Chiffre tmp = cf;
    tmp += i;   // on utilise l'opérateur += créé au dessus
    return tmp;
}

// Opérateur * pour Chiffre
Nombre::Chiffre operator *(const Nombre::Chiffre & cf, unsigned int i){
    Nombre::Chiffre tmp = cf;
    tmp *= i;   // on utilise l'opérateur *= créé au dessus
    return tmp;
}


Nombre factorielle(unsigned int n){
    Nombre fac{1uL};
    for(unsigned int i = 1u; i <= n; ++i){
        fac *= i;
    }
    return fac;
}

int main() {
    std::cout << "Hello World!";
    return 0;
}
