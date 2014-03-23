#ifndef PELAAJA_HPP
#define PELAAJA_HPP

#include <string>

#include "apuvalineet.hpp"

class pelaaja {
    int id;
    std::string tunnus;
    std::string etunimi;
    std::string sukunimi;
public:
    pelaaja(int id, std::string tunnus, std::string etunimi, std::string sukunimi);
    pelaaja(int id, std::string tunnus);
    pelaaja(){}
    std::string anna_etunimi();
    std::string anna_sukunimi();
    std::string anna_tunnus();
    int anna_id();

    bool operator ==(std::string tunnus) {
        return tunnus == this->tunnus;
    }
};

#endif // PELAAJA_HPP
