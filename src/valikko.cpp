// valikko.cpp
#include "valikko.hpp"
#include "ohjelma.hpp"
#include <iostream>

valikko::valinta valikko::aja(int pelin_tulos) {
	std::clog << "valikko::aja(" << pelin_tulos << ")" << std::endl;
	
	// Valikon alkutilanne.
	valinta valittu = PELI;
	
	// Valikon silmukka.
	while (true) {
		// Piirretään valikon tilanne, odotetaan valintaa.
		ohjelma::piirra_valikko(pelin_tulos, valittu);
		ohjelma::nappi n = ohjelma::odota_nappi();
		
		if (n == ohjelma::NAPPI_ENTER) {
			// Enter => lopetetaan valikko, palautetaan valittu.
			return valittu;
		} else if (n == ohjelma::NAPPI_ESCAPE) {
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
