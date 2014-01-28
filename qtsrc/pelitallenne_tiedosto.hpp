#ifndef TIEDOSTOPELITALLENNE_HPP
#define TIEDOSTOPELITALLENNE_HPP

#include "pelitallenne.hpp"
#include <iostream>
#include <fstream>

class TiedostoPeliTallenne : public IPeliTallenne {
public:
    TiedostoPeliTallenne(Peli& p);
    void tallenna();
    void lataa();
};

#endif // TIEDOSTOPELITALLENNE_HPP
