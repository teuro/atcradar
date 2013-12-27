#ifndef _SDLPIIRTOPINTA_HPP
#define _SDLPIIRTOPINTA_HPP

#include "asetukset.h"
#include "piirtopinta.hpp"

class SDLPiirtoPinta : public IPiirtoPinta
{
public:
	SDLPiirtoPinta(IAsetukset& a);

	void rectangleColor(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	void lineColor(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
	void circleColor(Sint16 x, Sint16 y, Sint16 rad, Uint32 color);
	void trigonColor(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3, Uint32 color);

	void piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta = false);
	void kirjoita_tekstia(std::string teksti, int x, int y, bool aktiivinen = false);

	int get_fontinkoko();
	void flip();
private:
	IAsetukset& asetukset;
	TTF_Font* fontti;
	SDL_Color vari;
	SDL_Surface *ruutu;
	int fontin_koko;
};

#endif