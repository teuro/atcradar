#include "pelaaja.hpp"

pelaaja::pelaaja(int id, std::string etunimi, std::string sukunimi) {
    this->id = id;
    this->etunimi = etunimi;
    this->sukunimi = sukunimi;
}

std::string pelaaja::anna_etunimi() {
    return this->etunimi;
}

std::string pelaaja::anna_sukunimi() {
    return this->sukunimi;
}
