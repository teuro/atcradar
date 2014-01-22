#include "navipiste.hpp"

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta) {
std::clog << "Navipiste::navipiste(" << nimi << ", " << paikka.x << ", " << paikka.y << ", " << korkeus << ", " << nopeus << ", " << suunta << ")" << std::endl;
    this->nimi = nimi;
	this->paikka = paikka;
	this->lentokorkeus = korkeus;
	this->lentonopeus = nopeus;
	this->lentosuunta = suunta;
}

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka) {
    std::clog << "Navipiste::navipiste(" << nimi << ", " << paikka.x << ", " << paikka.y << ")" << std::endl;
	this->nimi = nimi;
	this->paikka = paikka;
}

navipiste::navipiste();
