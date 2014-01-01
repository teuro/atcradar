// ohjelma.hpp
#ifndef _OHJELMA_HPP
#define _OHJELMA_HPP

#include "valikko.hpp"
#include "peli.hpp"
#include "apuvalineet.hpp"

#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

class IOhjelma
{
public:
	virtual double sekunnit(bool nollaa = false) = 0;
	virtual void odota(unsigned int ms = 1) = 0;

	enum nappi {
		NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU, NAPPI_F5, NAPPI_F7, NAPPI_F8, NAPPI_YLOS, NAPPI_ALAS, NAPPI_I
	};

	virtual nappi odota_nappi() = 0;
	virtual bool lue_nappi(nappi n) = 0;
	virtual nappi lue_nappi() = 0;
	virtual void tyhjenna_syote() = 0;

	virtual apuvalineet::piste anna_hiiri() = 0;
	virtual bool lue_hiiri() = 0;
	virtual std::string lue_syote() = 0;
	virtual void tyhjenna_viesti() = 0;
	virtual std::string anna_viesti() = 0;
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

	double sekunnit(bool nollaa = false);
	void odota(unsigned int ms = 1);

	nappi odota_nappi();
	bool lue_nappi(nappi n);
	nappi lue_nappi();
	void tyhjenna_syote();

	apuvalineet::piste anna_hiiri();
	bool lue_hiiri();
	std::string lue_syote();
	std::string anna_viesti() { return syote; }
	void tyhjenna_viesti() { syote = ""; }

private:
	IAsetukset& asetukset;
	unsigned int alku_aika;
	void alku();
	void loppu();
	std::string syote;
	void process_keyboard(bool wait);
	std::list<nappi> keyboard_buffer;
};

class Versio
{
public:
    std::string anna_versio();
};

#endif
