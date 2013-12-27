#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP

#include "kieli.hpp"

class PeliView;
class TilastoView;
class Peli;
class IOhjelma;
class IAsetukset;

/*
MVC Controller
PeliController handles and parses game input (from Ohjelma-class), game logic, updates game state (in Peli) and updates the display (using PeliView).
*/

class PeliController {
	Peli& peli;
	PeliView& view;
	TilastoView& tilastoview;
	IOhjelma& ohjelma;
	IAsetukset& asetukset;
	Kieli& kieli;
public:
	PeliController(Peli& p, PeliView& v, TilastoView& tv, IOhjelma& o, IAsetukset& a, Kieli& k) : peli(p), view(v), tilastoview(tv), ohjelma(o), asetukset(a), kieli(k) { }
	int aja();
	void pyyda_atis();
};
#endif
