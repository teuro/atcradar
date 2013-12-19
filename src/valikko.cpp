// valikko.cpp
#include "valikko.hpp"
#include "ohjelma.hpp"
#include "peliview.hpp"
#include <iostream>

valikko::valinta valikko::aja() {
	std::clog << "valikko::aja( )" << std::endl;

	// Valikon alkutilanne.
	valinta valittu = PELI;

	// Valikon silmukka.
	while (true) {
		// Piirretään valikon tilanne, odotetaan valintaa.
		view.piirra_valikko(valittu);
		Ohjelma::nappi n = ohjelma.odota_nappi();

		if (n == Ohjelma::NAPPI_ENTER) {
			// Enter => lopetetaan valikko, palautetaan valittu.
			return valittu;
		} else if (n == Ohjelma::NAPPI_ESCAPE) {
			// Escape => lopetetaan valikko, palautetaan LOPETUS.
			return LOPETUS;
		} else {
			// Muu nappi => vaihdetaan valintaa.
			if (valittu == PELI) {
				valittu = LOPETUS;
			} else {
				valittu = PELI;
			}
		}
	}
}
