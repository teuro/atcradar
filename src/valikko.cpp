// valikko.cpp
#include "valikko.hpp"
#include "ohjelma.hpp"
#include "peliview.hpp"
#include <iostream>

template <typename Iter>
Iter next_it(Iter iter)
{
	return ++iter;
}

template <typename Iter, typename Cont>
bool is_last(Iter iter, const Cont& cont)
{
	return (iter != cont.end()) && (next_it(iter) == cont.end());
}

int valikko::aja() {
	std::clog << "valikko::aja( )" << std::endl;

	// Valikon alkutilanne.
	std::map <int, std::string> :: iterator valinta = data.kohdat.begin();
	// Valikon silmukka.
	while (true) {
		data.valittuRivi = valinta->first;
		// Piirretään valikon tilanne, odotetaan valintaa.
		view.piirra();
		//view.piirra_valikko(valinta->first, this->kohdat);
		Ohjelma::nappi n = ohjelma.odota_nappi();

		if (n == Ohjelma::NAPPI_ENTER) {
			// Enter => lopetetaan valikko, palautetaan valinta.
			return valinta->first;
		} else if (n == Ohjelma::NAPPI_YLOS && valinta != data.kohdat.begin()) {
			--valinta;
		}
		else if (n == Ohjelma::NAPPI_ALAS && !is_last(valinta, data.kohdat)) {
			valinta++;
		}
	}
}
