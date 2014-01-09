#include "navipiste.hpp"

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta) {
	this->nimi = nimi;
	this->paikka = paikka;
	this->lentokorkeus = korkeus;
	this->lentonopeus = nopeus;
	this->lentosuunta = suunta;
}

navipiste::navipiste(std::string nimi, apuvalineet::piste paikka) {
	this->nimi = nimi;
	this->paikka = paikka;
}

navipiste::navipiste();
