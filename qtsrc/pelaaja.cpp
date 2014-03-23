#include "pelaaja.hpp"

pelaaja::pelaaja(int id, std::string tunnus, std::string etunimi, std::string sukunimi) {
    this->id = id;
    this->tunnus = tunnus;
    this->etunimi = etunimi;
    this->sukunimi = sukunimi;
}

pelaaja::pelaaja(int id, std::string tunnus) {
    this->id = id;
    this->tunnus = tunnus;
}

std::string pelaaja::anna_etunimi() {
    return this->etunimi;
}

std::string pelaaja::anna_sukunimi() {
    return this->sukunimi;
}

std::string pelaaja::anna_tunnus() {
    return this->tunnus;
}

int pelaaja::anna_id() {
    return this->id;
}
