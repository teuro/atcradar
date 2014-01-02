#ifndef _ATIS_CONTROLLER_HPP
#define _ATIS_CONTROLLER_HPP

#include "controller.hpp"
#include "view.hpp"
#include "peli.hpp"
#include "kieli.hpp"
#include "ohjelma.hpp"

class AtisController : IController
{
public:
	AtisController(IAsetukset& a, Kieli& k, IOhjelma& o, View& v, Peli& data, IPiirtoPinta& p) : peli(data), kieli(k), ohjelma(o), asetukset(a), view(v), piirtopinta(p) { }
	int aja();

	Peli &peli;
private:
	Kieli& kieli;
	IOhjelma& ohjelma;
	IAsetukset& asetukset;
	View& view;
    IPiirtoPinta& piirtopinta;
};

#endif
