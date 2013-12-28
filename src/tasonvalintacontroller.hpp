#ifndef _TASONVALINTA_CONTROLLER_HPP
#define _TASONVALINTA_CONTROLLER_HPP

#include "controller.hpp"
#include "view.hpp"
#include "kieli.hpp"
#include "ohjelma.hpp"

class TasonValintaController : IController
{
public:
	TasonValintaController(IAsetukset& a, Kieli& k, IOhjelma& o, View& v, std::string& data) : kieli(k), ohjelma(o), asetukset(a), view(v), teksti(data) { }
	int aja();

	std::string& teksti;
private:
	Kieli& kieli;
	IOhjelma& ohjelma;
	IAsetukset& asetukset;
	View& view;
};

#endif
