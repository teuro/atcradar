#ifndef _VALIKKOVIEW_HPP
#define _VALIKKOVIEW_HPP

#include "view.hpp"
#include <map>
#include <string>

class ValikkoData
{
public:
	int valittuRivi;
	std::map <int, std::string> kohdat;

	void lisaa_kohta(int id, std::string kohta) {
		kohdat[id] = kohta;
	}
};

class ValikkoView : public View
{
public:
	ValikkoView(ValikkoData& d) : data(d) { }
	void piirra(IPiirtoPinta& piirtopinta);

private:
	ValikkoData& data;
};

#endif
