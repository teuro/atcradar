#ifndef PELAAJA_HPP
#define PELAAJA_HPP

#include <string>

#include "apuvalineet.hpp"

class pelaaja {
    int id;
    std::string etunimi;
    std::string sukunimi;
public:
    pelaaja(int id, std::string etunimi, std::string sukunimi);
    std::string anna_etunimi();
    std::string anna_sukunimi();

    pelaaja operator ==(int id) {
        if (id == this->id) {
            return *(this);
        }
    }
};

#endif // PELAAJA_HPP
