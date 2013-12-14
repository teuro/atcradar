#ifndef _PELIVIEW_HPP
#define _PELIVIEW_HPP

#include "valikko.hpp"
#include "peli.hpp"
#include "apuvalineet.hpp"

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

class PeliView
{
public:
	PeliView(Peli &p, Ohjelma& o) : peli(p), ohjelma(o)
	{
		SDL_Color tmp = { 50, 50, 50 };
		vari = tmp;
		alku();
	}

	~PeliView()
	{
		// TODO: Cleanup any SDL resources in destructor
	}

	void piirra_valikko(int pelin_tulos, valikko::valinta valittu);
	void piirra_peli();
	void piirra_tilasto();
	void piirra_atis(int toiminto);

	std::map <std::string, double> asetukset;
private:
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

	int fontin_koko = 12;
	TTF_Font* fontti;
	SDL_Color vari;
	SDL_Surface *ruutu;
	Peli& peli;
	Ohjelma& ohjelma;

};

#endif