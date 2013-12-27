#ifndef _VIEW_HPP
#define _VIEW_HPP

#include "asetukset.h"
#include "piirtopinta.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

class View
{
protected:
	IPiirtoPinta& piirtopinta;

public:
	View(IPiirtoPinta& pinta) : piirtopinta(pinta) { }
};


#endif
