#ifndef _VIEW_HPP
#define _VIEW_HPP

#include "asetukset.h"
#include "piirtopinta.hpp"

class View
{
public:
	View(IPiirtoPinta& pinta) : piirtopinta(pinta) { }

	IPiirtoPinta& piirtopinta;
	virtual void piirra() = 0;
};


#endif
