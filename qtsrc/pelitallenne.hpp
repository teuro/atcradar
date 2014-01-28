#ifndef PELITALLENNE_HPP
#define PELITALLENNE_HPP

#include "peli.hpp"

class IPeliTallenne {
protected:
    IPeliTallenne(Peli& p) : peli(p) { }
public:
    Peli& peli;
    virtual void tallenna();
    virtual void lataa();
};

#endif // PELITALLENNE_HPP
