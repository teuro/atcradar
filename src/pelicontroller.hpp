#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP

#include "controller.hpp"
#include "ohjelma.hpp"
#include "asetukset.h"

class PeliView;
class Peli;
class IAsetukset;

/*
MVC Controller
PeliController handles and parses game input (from Ohjelma-class), game logic, updates game state (in Peli) and updates the display (using PeliView).
*/

class PeliController : public IController {
	Peli& peli;
	View& view;
	IOhjelma& ohjelma;
	IAsetukset& asetukset;
    IPiirtoPinta& piirtopinta;
public:
    PeliController(Peli& p, View& v, IOhjelma& o, IAsetukset& a, IPiirtoPinta& pinta) : peli(p), view(v), ohjelma(o), asetukset(a), piirtopinta(pinta)  {}
	
	bool kasittele_aikaa(double sekunnit);
	void kasittele_hiiren_nappi(apuvalineet::piste koordinaatit);
	void kasittele_hiiren_paikka(apuvalineet::piste koordinaatit);
	bool kasittele_nappi(IOhjelma::nappi nappi);
	bool kasittele_komento(const std::string& komento);

	void pyyda_atis();
	Peli::selvitys anna_selvitys(std::string komento, int toiminto);
private:

	void logita_peliajat();
	double pelin_kello;
};
#endif
