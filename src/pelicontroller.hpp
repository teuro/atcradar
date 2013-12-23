#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP
class PeliView;
class Peli;
class Ohjelma;
class IAsetukset;

/*
MVC Controller
PeliController handles and parses game input (from Ohjelma-class), game logic, updates game state (in Peli) and updates the display (using PeliView).
*/

class PeliController {
	Peli& peli;
	PeliView& view;
	Ohjelma& ohjelma;
	IAsetukset& asetukset;
public:
	PeliController(Peli& p, PeliView& v, Ohjelma& o, IAsetukset& a) : peli(p), view(v), ohjelma(o), asetukset(a) { }
	int aja();
	void pyyda_atis();
};
#endif
