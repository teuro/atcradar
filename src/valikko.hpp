// valikko.hpp
#ifndef _VALIKKO_HPP
#define _VALIKKO_HPP

#include <map>
#include "valikkoview.hpp"

class IOhjelma;
class PeliView;

class valikko {
public:
	valikko(IOhjelma& o, View& v, ValikkoData& d, IPiirtoPinta& p) : piirtopinta(p), ohjelma(o), view(v), data(d) {}
	
	int aja();
private:
    IPiirtoPinta &piirtopinta;
	IOhjelma& ohjelma;
	View& view;
	ValikkoData& data;
};

#endif
