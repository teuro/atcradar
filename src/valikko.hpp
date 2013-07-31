// valikko.hpp
#ifndef _VALIKKO_HPP
#define _VALIKKO_HPP

namespace valikko {
	enum valinta {
		LOPETUS, PELI
	};
	// Valikon pääfunktio; tarvitsee pelin tuloksen, palauttaa valinnan.
	valinta aja(int pelin_tulos);
}

#endif
