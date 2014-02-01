#ifndef TALLENNA_HPP
#define TALLENNA_HPP

#include <iostream>
#include "peli.hpp"

class Tallenna {
    Peli& peli;
public:
    Tallenna(Peli& p);
    ~Tallenna();
    void tallenna();
    void lataa();
};

#endif // TALLENNA_HPP
