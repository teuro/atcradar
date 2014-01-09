#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

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
    enum nappi {
        NAPPI_VASEN, NAPPI_OIKEA, NAPPI_ENTER, NAPPI_ESCAPE, NAPPI_MUU, NAPPI_F5, NAPPI_F7, NAPPI_F8, NAPPI_YLOS, NAPPI_ALAS, NAPPI_I
    };
public:
    PeliController(Peli& p, IAsetukset& a, IPiirtoPinta& pinta) : peli(p), asetukset(a), piirtopinta(pinta)  {}
	
	bool kasittele_aikaa(double sekunnit);
	void kasittele_hiiren_nappi(apuvalineet::piste koordinaatit);
	void kasittele_hiiren_paikka(apuvalineet::piste koordinaatit);
    bool kasittele_nappi(PeliController::nappi nappi);

	bool kasittele_komento(const std::string& komento);

	void pyyda_atis();

	Peli::selvitys anna_selvitys(std::string komento, int toiminto);

    void ota_aika(double aika) {
        this->pelin_kello = aika;
    }

private:

	void logita_peliajat();
	double pelin_kello;
};

#endif
