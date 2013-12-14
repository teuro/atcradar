#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP
class PeliView;
class Peli;
class Ohjelma;

/*
MVC Controller
PeliController handles and parses game input (from Ohjelma-class), game logic, updates game state (in Peli) and updates the display (using PeliView).
*/

class PeliController {
	Peli& peli;
	PeliView& view;
	Ohjelma& ohjelma;
public:
	PeliController(Peli& p, PeliView& v, Ohjelma& o) : peli(p), view(v), ohjelma(o) { }
	int aja();
	void pyyda_atis();
};
#endif
