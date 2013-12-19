// valikko.hpp
#ifndef _VALIKKO_HPP
#define _VALIKKO_HPP

class Ohjelma;
class PeliView;

class valikko {
    Ohjelma& ohjelma;
    PeliView& view;

public:
    valikko(Ohjelma& o, PeliView& v) : ohjelma(o), view(v) { }
	
	enum valinta {
		LOPETUS, PELI
	};
	
	// Valikon p‰‰funktio; tarvitsee pelin tuloksen, palauttaa valinnan.
	valinta aja();
};

#endif
