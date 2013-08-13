// ohjelma.hpp
#ifndef _OHJELMA_HPP
#define _OHJELMA_HPP

#include "valikko.hpp"
#include "peli.hpp"
#include "apuvalineet.hpp"

#include <map>
#include <fstream>
#include <iostream>
#include <cmath>

namespace ohjelma {
	void alku();
	void loppu();

	float sekunnit(bool nollaa = false);
	void odota(double ms = 1);

	enum nappi {
		NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU, NAPPI_F5, NAPPI_F7, NAPPI_F8, NAPPI_I
	};

	extern std::map <std::string, double> asetukset;
	double anna_asetus(std::string asetus);
	void lataa_asetukset(std::string nimi);

	nappi odota_nappi();
	bool lue_nappi(nappi n);
	void tyhjenna_syote();

	void piirra_valikko(int pelin_tulos, valikko::valinta valittu);

	void piirra_peli();

	bool onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade = 0.2);
	apuvalineet::piste anna_hiiri();
	bool lue_hiiri();
	void piirra_tilasto();
	void piirra_atis(int toiminto);
	void kirjoita_tekstia(std::string teksti, int x, int y);

	namespace versio {
		std::string anna_versio();
	}
}

#endif
