#include "navipiste.hpp"

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi) {
    #ifdef DEBUG
        std::clog << "Navipiste::navipiste(" << nimi << ", " << paikka.x << ", " << paikka.y << ", " << korkeus << ", " << nopeus << ", " << suunta << ")" << std::endl;
    #endif

    this->nimi = nimi;
	this->paikka = paikka;
	this->lentokorkeus = korkeus;
	this->lentonopeus = nopeus;
    this->lentosuunta = suunta;
    this->tyyppi = tyyppi;
}

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka, int tyyppi) {
    #ifdef DEBUG
        std::clog << "Navipiste::navipiste(" << nimi << ", " << paikka.x << ", " << paikka.y << ", " << tyyppi << ")" << std::endl;
    #endif

    this->nimi = nimi;
    this->paikka = paikka;
    this->tyyppi = tyyppi;
}

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka) {
    #ifdef DEBUG
        std::clog << "Navipiste::navipiste(" << nimi << ", " << paikka.x << ", " << paikka.y << ")" << std::endl;
    #endif

	this->nimi = nimi;
	this->paikka = paikka;
}

navipiste::navipiste();
