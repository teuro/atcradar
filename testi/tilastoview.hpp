#ifndef _TILASTOVIEW_HPP
#define _TILASTOVIEW_HPP

#include "view.hpp"
#include "peli.hpp"

class TilastoView : public View {
public:
	TilastoView(Kieli& k, Peli& p) : peli(p), kieli(k) { }
	void piirra(IPiirtoPinta& piirtopinta);
private:
	Peli& peli;
	Kieli& kieli;
};

#endif
