#ifndef _PELIVIEW_HPP
#define _PELIVIEW_HPP

#include "valikko.hpp"
#include "peli.hpp"
#include "apuvalineet.hpp"
#include "kieli.hpp"
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
private:
	Kieli kieli;
public:
	PeliView(IAsetukset& a, Peli &p, IOhjelma& o, Kieli& kieli) : asetukset(a), peli(p), ohjelma(o) {
		this->kieli = kieli;
	}

	~PeliView()
	{
	}

	void piirra(IPiirtoPinta& pinta);
	//void piirra_atis();

private:
	IAsetukset& asetukset;

	void piirra_koneet(IPiirtoPinta& piirtopinta);
	void piirra_navipisteet(IPiirtoPinta& piirtopinta);
	void piirra_lentokentta(IPiirtoPinta& piirtopinta);
	void piirra_tilanne(IPiirtoPinta& piirtopinta);
	void piirra_metar(IPiirtoPinta& piirtopinta);
	void piirra_odottavat(IPiirtoPinta& piirtopinta);
	void piirra_ohje(IPiirtoPinta& piirtopinta, std::string ohje);
	Peli& peli;
	IOhjelma& ohjelma;
};

#endif
