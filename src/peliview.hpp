#ifndef _PELIVIEW_HPP
#define _PELIVIEW_HPP

#include "apuvalineet.hpp"
#include "view.hpp"

#include <map>
#include <fstream>
#include <iostream>
#include <cmath>

/*
MVC View
PeliView is responsible of DISPLAYING the game state. Nothing else.
*/

class PeliView : public View {

public:
    PeliView(IAsetukset& a, IOhjelma& o) : asetukset(a), ohjelma(o) { }

    ~PeliView() { }

	void piirra(IPiirtoPinta& pinta);

private:
	IAsetukset& asetukset;

	void piirra_koneet(IPiirtoPinta& piirtopinta);
	void piirra_navipisteet(IPiirtoPinta& piirtopinta);
	void piirra_lentokentta(IPiirtoPinta& piirtopinta);
	void piirra_tilanne(IPiirtoPinta& piirtopinta);
	void piirra_metar(IPiirtoPinta& piirtopinta);
	void piirra_odottavat(IPiirtoPinta& piirtopinta);
	void piirra_ohje(IPiirtoPinta& piirtopinta, std::string ohje);

	IOhjelma& ohjelma;
};

#endif
