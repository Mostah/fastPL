#include <iostream>

class Nombre {
public:
    Nombre();
    Nombre(unsigned long n);
    Nombre(const Nombre & nb); // Constructeur de copie
    ~Nombre();

    Nombre & operator =(const Nombre & nb);  // Opérateur d'affectation
    Nombre & operator +=(unsigned int i);
    Nombre & operator *=(unsigned int i);

    void swap(Nombre & nb); 
    friend std::ostream & operator <<( std::ostream & out, const Nombre & nb );
    friend std::istream & operator >>( std::istream & in, Nombre & nb );
    friend Nombre operator +(const Nombre & nb, unsigned int i);
    friend Nombre operator *(const Nombre & nb, unsigned int i);
 
//private:
    struct Chiffre {
        unsigned int chiffre_;     // entre 0 et 9
        Chiffre * suivant_;        // chiffre de poids supérieur ou nullptr

        Chiffre(unsigned long n);
        Chiffre(const Chiffre & cf);
        ~Chiffre();

        Chiffre & operator =(const Chiffre & cf);
        Chiffre & operator +=(unsigned int i);
        Chiffre & operator *=(unsigned int i);

        friend Chiffre operator +(const Chiffre & cf, unsigned int i);
        friend Chiffre operator *(const Chiffre & cf, unsigned int i);

        std::ostream& print(std::ostream& out);
        void swap(Chiffre & cf);   
    };
    Chiffre * premier_;        
};

Nombre factorielle(unsigned int n);