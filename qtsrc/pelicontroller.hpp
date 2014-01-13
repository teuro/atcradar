#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <list>

#include "controller.hpp"
#include "asetukset.h"
#include "peli.hpp"

class PeliView;
class Peli;
class IAsetukset;

/*
MVC Controller
PeliController handles and parses game input (from Ohjelma-class), game logic, updates game state (in Peli) and updates the display (using PeliView).
*/

class PeliController : public IController {
	Peli& peli;
	IAsetukset& asetukset;
    IPiirtoPinta& piirtopinta;
public:
    PeliController(Peli& p, IAsetukset& a, IPiirtoPinta& pinta) : peli(p), asetukset(a), piirtopinta(pinta)  {
        pelin_kello = 0;
        koska_uusi_kone = 150;
    }
	
	bool kasittele_aikaa(double sekunnit);
	void kasittele_hiiren_nappi(apuvalineet::piste koordinaatit);
	void kasittele_hiiren_paikka(apuvalineet::piste koordinaatit);

    bool kasittele_komento(const std::string& komento);

	void pyyda_atis();

    void anna_selvitys(std::string komento, int toiminto);

    void ota_aika(double aika) {
        this->pelin_kello = aika;
    }

private:

	void logita_peliajat();
	double pelin_kello;
    int koska_uusi_kone;
};

#endif
