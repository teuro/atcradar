#include "asetukset.hpp"

Asetukset::Asetukset() {
	lataa_asetukset("data/asetukset.ini");
}

void Asetukset::lataa_asetukset(std::string nimi) {
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedosto " + nimi + " ei aukea tai puuttuu");
	}

	std::string asetus_nimi;
	int asetus_arvo;

	while (sisaan >> asetus_nimi >> asetus_arvo) {
		Asetukset::asetukset[asetus_nimi] = asetus_arvo;
	}

	sisaan.close();
}

int Asetukset::anna_asetus(std::string asetus) {
	asetus_iterator onko;

	onko = asetukset.find(asetus);

	if (onko == Asetukset::asetukset.end()) {
		std::clog << "Asetusta " + asetus + " ei ole" << std::endl;
	}

	return asetukset[asetus];
}

void Asetukset::muuta_asetusta(std::string asetus, int arvo) {
	asetus_iterator onko;
	onko = asetukset.find(asetus);

	if (onko == Asetukset::asetukset.end()) {
		std::clog << "Asetusta " + asetus + " ei ole" << std::endl;
	}
	
	asetukset[asetus] = arvo;
}
