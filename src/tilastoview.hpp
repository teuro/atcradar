#ifndef _TILASTOVIEW_HPP
#define _TILASTOVIEW_HPP

#include "view.hpp"
#include "peli.hpp"

class TilastoView : public View {
public:
	TilastoView(Kieli& k, IPiirtoPinta& pn, Peli& p) : View(pn), peli(p), kieli(k) { }
	void piirra();
private:
	Peli& peli;
	Kieli& kieli;
};

#endif
