#include "asetukset.h"
#include "apuvalineet.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

std::map <std::string, double> Asetukset::asetukset;

void Asetukset::lataa_asetukset(std::string nimi) {
	std::clog << "asetukset::lataa_asetukset(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedosto " + nimi + " ei aukea tai puuttuu");
	}

	std::string asetus_nimi;
	double asetus_arvo;

	while (sisaan >> asetus_nimi >> asetus_arvo) {
		//std::clog << asetus_nimi << " " << asetus_arvo << std::endl;
		Asetukset::asetukset[asetus_nimi] = asetus_arvo;
	}

	sisaan.close();
}

double Asetukset::anna_asetus(std::string asetus) {
	//std::clog << "asetukset::anna_asetus(" << asetus << ")" << std::endl;
	std::map <std::string, double>::iterator onko;

	onko = asetukset.find(asetus);

	if (onko == Asetukset::asetukset.end()) {
		//throw std::logic_error("Asetusta " + asetus + " ei ole");
		std::clog << "Asetusta " + asetus + " ei ole" << std::endl;
	}

	return asetukset[asetus];
}

void Asetukset::muuta_asetusta(std::string asetus, double arvo) {
	std::map <std::string, double>::iterator onko;
	onko = asetukset.find(asetus);

	if (onko == Asetukset::asetukset.end()) {
		//throw std::logic_error("Asetusta " + asetus + " ei ole");
		std::clog << "Asetusta " + asetus + " ei ole" << std::endl;
	}
	
	asetukset[asetus] = arvo;
}
