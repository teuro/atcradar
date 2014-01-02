#ifndef __ATISVIEW_HPP
#define __ATISVIEW_HPP

#include "view.hpp"
#include "peli.hpp"
#include "kieli.hpp"

class AtisView : public View
{
public:
	AtisView(Kieli& k, Peli& data) : peli(data), kieli(k)  { }
	void piirra(IPiirtoPinta& piirtopinta);

private:
	// Reference to shared data between view and model/controller
	Peli& peli;
	Kieli& kieli;
	void piirra_metar(IPiirtoPinta& piirtopinta);
};

#endif
