#ifndef __ATISVIEW_HPP
#define __ATISVIEW_HPP

#include "view.hpp"
#include "peli.hpp"
#include "kieli.hpp"

class AtisView : public View
{
public:
	AtisView(IPiirtoPinta& pinta, Kieli& k, Peli& data) : peli(data), kieli(k), View(pinta) { }
	void piirra();

private:
	// Reference to shared data between view and model/controller
	Peli& peli;
	Kieli& kieli;
	void piirra_metar();
};

#endif
