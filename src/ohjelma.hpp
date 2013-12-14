// ohjelma.hpp
#ifndef _OHJELMA_HPP
#define _OHJELMA_HPP

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

class PeliView
{
    SDL_Surface *ruutu;
    Peli& peli;
    Ohjelma& ohjelma;

public:
    PeliView(Peli &p, Ohjelma& o) : peli(p), ohjelma(o) 
    {
        vari = { 50, 50, 50 };
    }

    void alku();
    void piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta = false);
    void piirra_koneet();
    void piirra_navipisteet();
    void piirra_lentokentta();
    void piirra_tilanne();
    int fontin_koko = 12;
    void piirra_ohje(std::string ohje);
    void piirra_metar();
    void piirra_odottavat();

    void piirra_valikko(int pelin_tulos, valikko::valinta valittu);
    void piirra_peli();
    void piirra_tilasto();
    void piirra_atis(int toiminto);
    void kirjoita_tekstia(std::string teksti, int x, int y);

    std::map <std::string, double> asetukset;

    TTF_Font* fontti;
    SDL_Color vari;
    //}
};

static class Asetukset
{
public:
    static std::map <std::string, double> asetukset;
    static double anna_asetus(std::string asetus);
    static void lataa_asetukset(std::string nimi);
};

class Ohjelma {
public:
	void alku();
	void loppu();

	float sekunnit(bool nollaa = false);
	void odota(double ms = 1);

	enum nappi {
		NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU, NAPPI_F5, NAPPI_F7, NAPPI_F8, NAPPI_I
	};

	nappi odota_nappi();
	bool lue_nappi(nappi n);
	void tyhjenna_syote();

	bool onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade = 0.2);
	apuvalineet::piste anna_hiiri();
	bool lue_hiiri();
};

class Versio 
{
    const int paa = 2;
    const int kehitys = 0;
    const int rakennus = 1;
public:
    std::string anna_versio();
};


#endif
