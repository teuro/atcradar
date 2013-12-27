// valikko.hpp
#ifndef _VALIKKO_HPP
#define _VALIKKO_HPP

#include <map>

class IOhjelma;
class PeliView;

class valikko {
    IOhjelma& ohjelma;
    PeliView& view;

public:
    valikko(IOhjelma& o, PeliView& v) : ohjelma(o), view(v) { }
	
	std::map <int, std::string> kohdat;
	void lisaa_kohta(int id, std::string kohta);
	
	int aja();
};

#endif
