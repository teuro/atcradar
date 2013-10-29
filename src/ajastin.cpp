#include "ajastin.hpp"

ajastin::ajastin(std::string nimi, int vali, int aika) {
	this->nimi = nimi;
	this->vali = vali;
	this->aika = aika + this->vali;
}

bool ajastin::onko_valmis(int leima) {
	return (this->aika < leima);
}

void ajastin::aseta_aika(int aika) {
	this->aika = aika;
}
