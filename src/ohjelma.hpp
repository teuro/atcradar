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

class Ohjelma {
public:
	Ohjelma()
	{
		alku();
	}

	~Ohjelma()
	{
		loppu();
	}

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
private:
	Uint32 alku_aika;
	void alku();
	void loppu();
};

class Versio 
{
public:
    std::string anna_versio();
};

#endif
