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
	PeliView(IPiirtoPinta& pinta, IAsetukset& a, Peli &p, IOhjelma& o, Kieli& kieli) : View(pinta), asetukset(a), peli(p), ohjelma(o) {
		this->kieli = kieli;
	}

	~PeliView()
	{
	}

	void piirra();
	//void piirra_atis();

private:
	IAsetukset& asetukset;

	void piirra_koneet();
	void piirra_navipisteet();
	void piirra_lentokentta();
	void piirra_tilanne();
	void piirra_metar();
	void piirra_odottavat();
	void piirra_ohje(std::string ohje);
	Peli& peli;
	IOhjelma& ohjelma;
};

#endif
