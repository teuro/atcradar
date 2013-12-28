#ifndef _VIEW_HPP
#define _VIEW_HPP

#include "asetukset.h"
#include "piirtopinta.hpp"

class View
{
protected:
	IPiirtoPinta& piirtopinta;

public:
	View(IPiirtoPinta& pinta) : piirtopinta(pinta) { }

	virtual void piirra() = 0;
};


#endif
