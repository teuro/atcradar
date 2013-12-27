#ifndef _PIIRTOPINTA_HPP
#define _PIIRTOPINTA_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

// Abstrakti piirtopinta, jota n‰kym‰t (View) voivat k‰ytt‰‰ piirt‰miseen
// TODO: Poista riippuvuudet SDL-tyyppeihin

class IPiirtoPinta
{
public:
	virtual void rectangleColor(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color) = 0;
	virtual void lineColor(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color) = 0;
	virtual void circleColor(Sint16 x, Sint16 y, Sint16 rad, Uint32 color) = 0;
	virtual void trigonColor(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color) = 0;

	virtual void piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta = false) = 0;
	virtual void kirjoita_tekstia(std::string teksti, int x, int y, bool aktiivinen = false) = 0;

	virtual int get_fontinkoko() = 0;
	virtual void flip() = 0;
};

#endif