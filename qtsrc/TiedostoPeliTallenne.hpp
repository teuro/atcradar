#ifndef TIEDOSTOPELITALLENNE_HPP
#define TIEDOSTOPELITALLENNE_HPP

#include "pelitallenne.hpp"
#include <iostream>
#include <fstream>

class TiedostoPeliTallenne : public IPeliTallenne {
public:
    void tallenna() {
        std::clog << "Peli tallenteen tallennus" << std::endl;
    }
};

#endif // TIEDOSTOPELITALLENNE_HPP
