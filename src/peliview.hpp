#ifndef _PELIVIEW_HPP
#define _PELIVIEW_HPP

#include "valikko.hpp"
#include "peli.hpp"
#include "apuvalineet.hpp"
#include "kieli.hpp"

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

class PeliView {
private:
	Kieli kieli;
public:
	PeliView(IAsetukset& a, Peli &p, IOhjelma& o, Kieli& kieli) : asetukset(a), peli(p), ohjelma(o) {
		SDL_Color tmp = { 50, 50, 50 };
		vari = tmp;
		fontin_koko = 12;

		alku();
		this->kieli = kieli;
	}

	~PeliView()
	{
		// TODO: Cleanup any SDL resources in destructor
	}

	void piirra_valikko(valikko::valinta valittu);
	void piirra_peli();
	void piirra_tilasto();
	void piirra_atis();
	void piirra_valinta();

private:
	IAsetukset& asetukset;

	void alku();

	void piirra_koneet();
	void piirra_navipisteet();
	void piirra_lentokentta();
	void piirra_tilanne();
	void piirra_metar();
	void piirra_odottavat();
	void piirra_ohje(std::string ohje);
	void piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta = false);
	void kirjoita_tekstia(std::string teksti, int x, int y);

	int fontin_koko;
	TTF_Font* fontti;
	SDL_Color vari;
	SDL_Surface *ruutu;
	Peli& peli;
	IOhjelma& ohjelma;
};

#endif
