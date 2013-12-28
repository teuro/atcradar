#ifndef _SDLPIIRTOPINTA_HPP
#define _SDLPIIRTOPINTA_HPP

#include "asetukset.h"
#include "piirtopinta.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

class SDLPiirtoPinta : public IPiirtoPinta
{
public:
	SDLPiirtoPinta(IAsetukset& a);

	void rectangleColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color);
	void lineColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color);
	void circleColor(unsigned short x, unsigned short y, unsigned short rad, unsigned int color);
	void trigonColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, unsigned int color);

	void piirra_kuva(const char* tiedosto, int x, int y, bool keskikohta = false);
	void kirjoita_tekstia(std::string teksti, int x, int y, bool aktiivinen = false);

	int get_fontinkoko();
	void flip();
private:
	IAsetukset& asetukset;
	TTF_Font* fontti;
	SDL_Color vari;
	SDL_Surface *ruutu;
	int fontin_koko;

	void piirra_kuva(SDL_Surface* pinta, int x, int y, bool keskikohta = false);
};

#endif