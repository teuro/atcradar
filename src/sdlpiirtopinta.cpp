#include "sdlpiirtopinta.hpp"
#include "kuvavarasto.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <iostream>

SDLPiirtoPinta::SDLPiirtoPinta(IAsetukset& a) : asetukset(a) {
	SDL_Color tmp = { 50, 50, 50 };
	vari = tmp;
	fontin_koko = 12;

	// Avataan ikkuna tai heitetään virhe.
	ruutu = SDL_SetVideoMode(asetukset.anna_asetus("ruutu_leveys"), asetukset.anna_asetus("ruutu_korkeus"), 32, SDL_DOUBLEBUF);

	if (!ruutu) {
		throw std::runtime_error(SDL_GetError());
	}

	// Asetetaan otsikko.
	SDL_WM_SetCaption("ATC-radar", "ATC-radar");

	if (TTF_Init() != 0) {
		throw std::runtime_error(TTF_GetError());
	}

	fontti = TTF_OpenFont("fontit/arial.ttf", fontin_koko);
	if (fontti == NULL) {
		throw std::runtime_error("Cannot open font");
	}
}

int SDLPiirtoPinta::get_fontinkoko()
{
	return fontin_koko;
}

void SDLPiirtoPinta::piirra_kuva(const char* tiedosto, int x, int y, bool keskikohta) {
	SDL_Surface* kuva = image_cache::common().get(tiedosto);
	piirra_kuva(kuva, x, y, keskikohta);
}

void SDLPiirtoPinta::piirra_kuva(SDL_Surface* kuva, int x, int y, bool keskikohta)
{
	SDL_Rect r = { (Sint16)x, (Sint16)y };

	if (keskikohta) {
		r.x -= kuva->w / 2;
		r.y -= kuva->h / 2;
	}

	SDL_BlitSurface(kuva, 0, ruutu, &r);
}

void SDLPiirtoPinta::flip()
{
	SDL_Flip(ruutu);
}

void SDLPiirtoPinta::kirjoita_tekstia(std::string teksti, int x, int y, bool aktiivinen) {
	if (aktiivinen) {
		vari = { 0, 128, 0 };
	}
	else {
		vari = { 128, 0, 0 };
	}

	if (teksti.length() > 0) {
		SDL_Surface* tekstipinta = TTF_RenderText_Solid(fontti, teksti.c_str(), vari);

		if (!tekstipinta) {
			throw std::runtime_error(SDL_GetError());
		}

		piirra_kuva(tekstipinta, x, y, false);
		SDL_FreeSurface(tekstipinta);
	}
}

void SDLPiirtoPinta::rectangleColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color)
{
	::rectangleColor(ruutu, x1, y1, x2, y2, color);
}

void SDLPiirtoPinta::lineColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color)
{
	::lineColor(ruutu, x1, y1, x2, y2, color);
}

void SDLPiirtoPinta::circleColor(unsigned short x, unsigned short y, unsigned short rad, unsigned int color)
{
	::circleColor(ruutu, x, y, rad, color);
}

void SDLPiirtoPinta::trigonColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, unsigned int color)
{
	::trigonColor(ruutu, x1, y1, x2, y2, x3, y3, color);
}
