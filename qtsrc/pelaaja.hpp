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
};

#endif // PELAAJA_HPP
