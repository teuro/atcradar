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
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

/*
MVC View
PeliView is responsible of DISPLAYING the game state. Nothing else.
*/

class PeliView : public View {
private:
	Kieli kieli;
public:
	PeliView(IPiirtoPinta& pinta, IAsetukset& a, Peli &p, IOhjelma& o, Kieli& kieli) : asetukset(a), peli(p), ohjelma(o), View(pinta) {
		this->kieli = kieli;
	}

	~PeliView()
	{
	}

	void piirra_valikko(int valittu, std::map<int, std::string>kohdat);
	void piirra_peli();
	void piirra_tilasto();
	void piirra_atis();
	void piirra_valinta();

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
