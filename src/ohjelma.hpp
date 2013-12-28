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

class IOhjelma
{
public:
	virtual float sekunnit(bool nollaa = false) = 0;
	virtual void odota(double ms = 1) = 0;

	enum nappi {
		NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU, NAPPI_F5, NAPPI_F7, NAPPI_F8, NAPPI_YLOS, NAPPI_ALAS, NAPPI_I
	};

	virtual nappi odota_nappi() = 0;
	virtual bool lue_nappi(nappi n) = 0;
	virtual void tyhjenna_syote() = 0;

	virtual apuvalineet::piste anna_hiiri() = 0;
	virtual bool lue_hiiri() = 0;
};

class Ohjelma : public IOhjelma {
public:
	Ohjelma(IAsetukset &a) : asetukset(a) {
		alku();
	}

	~Ohjelma()
	{
		loppu();
	}

	float sekunnit(bool nollaa = false);
	void odota(double ms = 1);

	nappi odota_nappi();
	bool lue_nappi(nappi n);
	void tyhjenna_syote();

	apuvalineet::piste anna_hiiri();
	bool lue_hiiri();

private:
	IAsetukset& asetukset;
	unsigned int alku_aika;
	void alku();
	void loppu();
};

class Versio
{
public:
    std::string anna_versio();
};

#endif
