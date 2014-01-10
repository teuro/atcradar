#include "Selvitys.hpp"

selvitys::selvitys(double suunta, double korkeus, double nopeus, double aika, int kaarto, navipiste kohde) {
    this->suunta = suunta;
    this->korkeus = korkeus;
    this->nopeus = nopeus;
    this->aika = aika;
    this->kaarto = kaarto;
    this->kohde = kohde;
}

double selvitys::anna_suunta() {
    return this->suunta;
}

double selvitys::anna_korkeus() {
    return this->korkeus;
}

double selvitys::anna_nopeus() {
    return this->nopeus;
}

navipiste selvitys::anna_kohde() {
    return this->kohde;
}

double selvitys::anna_aika() {
    return this->aika;
}

int selvitys::anna_kaarto() {
    return this->kaarto;
}

void selvitys::aseta_suunta(double suunta) {
    this->suunta = suunta;
}

void selvitys::aseta_korkeus(double korkeus) {
    this->korkeus = korkeus;
}

void selvitys::aseta_nopeus(double nopeus) {
    this->nopeus = nopeus;
}

void selvitys::aseta_kohde(navipiste kohde) {
    this->kohde = kohde;
}

void selvitys::aseta_aika(double aika) {
    this->aika = aika;
}

void selvitys::aseta_kaarto(int kaarto) {
    this->kaarto = kaarto;
}
