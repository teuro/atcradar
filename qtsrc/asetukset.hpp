#ifndef __ASETUKSET_H
#define __ASETUKSET_H

#include "apuvalineet.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <map>
#include <string>

class IAsetukset
{
public:
    virtual int anna_asetus(std::string asetus) = 0;
	virtual void lataa_asetukset(std::string nimi) = 0;
    virtual void muuta_asetusta(std::string asetus, int arvo) = 0;
};

// TODO: Voisi harkita että asetukset-oliolla olisi vaan membereinä
// asetukset, jotta niitä ei tarvitsisi referoida virhealttiilla merkkijonoilla.
// Esim. asetukset.koska_metar eikä asetukset.anna_asetus("koska_metar")
// Sitten olisi erikseen AsetusLukija, joka lukisi asetukset tiedostosta.
class Asetukset : public IAsetukset {
public:
	Asetukset();
    int anna_asetus(std::string asetus);
	void lataa_asetukset(std::string nimi);
    void muuta_asetusta(std::string asetus, int arvo);
private:
    std::map <std::string, int> asetukset;

    typedef std::map <std::string, int>::iterator asetus_iterator;
};

#endif
