// valikko.cpp
#include "valikko.hpp"
#include "ohjelma.hpp"
#include "peliview.hpp"
#include <iostream>

int valikko::aja() {
	std::clog << "valikko::aja( )" << std::endl;

	// Valikon alkutilanne.
	std::map <int, std::string> :: iterator valinta = kohdat.begin();
	// Valikon silmukka.
	while (true) {
		// Piirretään valikon tilanne, odotetaan valintaa.
		view.piirra_valikko(valinta->first, this->kohdat);
		Ohjelma::nappi n = ohjelma.odota_nappi();

		if (n == Ohjelma::NAPPI_ENTER) {
			// Enter => lopetetaan valikko, palautetaan valinta.
			return valinta->first;
		} else if (n == Ohjelma::NAPPI_YLOS) {
			--valinta;
		} else if (n == Ohjelma::NAPPI_ALAS) {
			++valinta;
		}
	}
}

void valikko::lisaa_kohta(int id, std::string kohta) {
	kohdat[id] = kohta;
}
