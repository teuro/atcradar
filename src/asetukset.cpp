#include "asetukset.h"
#include "apuvalineet.hpp"
#include <iostream>
#include <fstream>
#include <string>

std::map <std::string, double> Asetukset::asetukset;

void Asetukset::lataa_asetukset(std::string nimi) {
	std::clog << "ohjelma::lataa_asetukset(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedosto " + nimi + " ei aukea tai puuttuu");
	}

	std::string asetus_nimi;
	double asetus_arvo;

	while (sisaan >> asetus_nimi >> asetus_arvo) {
		Asetukset::asetukset[asetus_nimi] = asetus_arvo;
	}

	// TODO: fix this without dependency on peli
	//Asetukset::asetukset["maks_lahestymiskorkeus"] = apuvalineet::mi2ft(peli.kentta.korkeus + std::tan(apuvalineet::deg2rad(Asetukset::asetukset["liukupolku"])) * Asetukset::asetukset["lahestymispiste"]);
	//std::clog << "Lahestymisen korkeus lahestymispisteella on " << Asetukset::asetukset["maks_lahestymiskorkeus"] << std::endl;

	sisaan.close();
}

double Asetukset::anna_asetus(std::string asetus) {
	std::map <std::string, double>::iterator onko;

	onko = asetukset.find(asetus);

	if (onko == Asetukset::asetukset.end()) {
		throw std::logic_error("Asetusta " + asetus + " ei ole");
	}

	return asetukset[asetus];
}
