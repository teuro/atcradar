#ifndef __TASONVALINTAVIEW_HPP
#define __TASONVALINTAVIEW_HPP

#include "view.hpp"
#include "kieli.hpp"

class TasonValintaView : public View
{
public:
	TasonValintaView(IPiirtoPinta& pinta, Kieli& k, std::string& data) : View(pinta), teksti(data), kieli(k) { }
	void piirra();

private:
	// Reference to shared data between view and model/controller
	std::string& teksti;
	Kieli& kieli;
};

#endif
