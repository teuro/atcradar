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
		NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU, NAPPI_F5, NAPPI_F7, NAPPI_F8, NAPPI_I
	};

	virtual nappi odota_nappi() = 0;
	virtual bool lue_nappi(nappi n) = 0;
	virtual void tyhjenna_syote() = 0;

	virtual bool onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade = 0.2) = 0;
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

	bool onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade = 0.2);
	apuvalineet::piste anna_hiiri();
	bool lue_hiiri();
	
	enum tekstit {
		TEKSTI_VALITSE_TASO = 1, 
		TEKSTI_PORRASTUSVIRHEET, 
		TEKSTI_UUSI_KONE_TULEE, 
		TEKSTI_SEKUNNIT,
		TEKSTI_OHJE_VALITSE_KONE, 
		TEKSTI_OHJE_PAINA_TOIMINTONAPPAINTA,
		TEKSTI_ONKO_ATIS_OK,
		TEKSTI_OHJE_SIIRTOPINTA_VAARIN,
		TEKSTI_OHJE_LAHTOBAANA_VAARIN,
		TEKSTI_OHJE_LASKUBAANA_VAARIN,
		TEKSTI_OHJE_ANNA_LAHTOKIITOTIE,
		TEKSTI_OHJE_ANNA_LASKUKIITOTIE,
		TEKSTI_OHJE_ANNA_SIIRTOPINTA,
		TEKSTI_OHJE_LAHTOKIITOTIE,
		TEKSTI_OHJE_LASKUKIITOTIE,
		TEKSTI_OHJE_SIIRTOPINTA,
		TEKSTI_OHJE_SUUNTA,
		TEKSTI_OHJE_NOPEUS,
		TEKSTI_OHJE_KORKEUS,
		TEKSTI_KUTSUTUNNUS,
		TEKSTI_ALUEELLE,
		TEKSTI_POIS,
		TEKSTI_ALUEELLA,
		TEKSTI_SELVITYKSET
	};
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
