// ohjelma.cpp
#include "asetukset.h"
#include "ohjelma.hpp"
#include "kuvavarasto.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

void Ohjelma::alku() {
	std::clog << "ohjelma::alku()" << std::endl;

	// Alustetaan SDL tai heitet��n virhe.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(SDL_GetError());
	}

	//asetukset.lataa_asetukset("data/asetukset.ini");

	SDL_EnableUNICODE(SDL_ENABLE);

	// Nollataan sekuntilaskuri.
	sekunnit(true);
	std::ofstream ulos("selvitykset.txt", std::ios::out);
	ulos << "";
	ulos.close();
}

// Lopetusfunktio.
void Ohjelma::loppu() {
	std::clog << "ohjelma::loppu()" << std::endl;

	// Suljetaan SDL.
	SDL_Quit();
}

// Kertoo nollauksesta kuluneiden sekuntien m��r�n.
float Ohjelma::sekunnit(bool nollaa) {
	Uint32 nyt = SDL_GetTicks();
	if (nollaa) {
		alku_aika = nyt;
	}
	return (nyt - alku_aika) / 1000.0f;
}

// Odottaa lyhyen ajan.
void Ohjelma::odota(unsigned int ms) {
	SDL_Delay(ms);
}

// Lukee seuraavan napinpainalluksen.
Ohjelma::nappi Ohjelma::odota_nappi() {
	// Odotellaan, kunnes tulee napinpainallus.
	SDL_Event e;
	while (SDL_WaitEvent(&e)) {
		if (e.type != SDL_KEYDOWN) continue;
		switch (e.key.keysym.sym) {
			case SDLK_ESCAPE: return NAPPI_ESCAPE;
			case SDLK_RETURN: return NAPPI_ENTER;
			case SDLK_LEFT: return NAPPI_VASEN;
			case SDLK_RIGHT: return NAPPI_OIKEA;
			case SDLK_DOWN: return NAPPI_ALAS;
			case SDLK_UP: return NAPPI_YLOS;
			default: return NAPPI_MUU;
		}
	}
	// Jokin meni pieleen!
	throw std::runtime_error(SDL_GetError());
}

std::string Ohjelma::lue_syote() {
	//peli::syote = this->str;
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_KEYDOWN)
	{
		//Keep a copy of the current version of the string
		std::string temp = syote;

		//If the string less than maximum size
		if (syote.length() <= 16)
		{
			if (event.key.keysym.unicode == (Uint16)' ' ||
				(event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9') ||
				(event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'�') ||
				(event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z'))
			{
				//Append the character
				syote += (char)event.key.keysym.unicode;
			}
		}
		//If backspace was pressed and the string isn't blank
		if ((event.key.keysym.sym == SDLK_BACKSPACE) && (syote.length() != 0))
		{
			//Remove a character from the end
			syote.erase(syote.length() - 1);
		}
	}
	return syote;
}

// Kertoo napin nykytilan.
bool Ohjelma::lue_nappi(nappi n) {
	// K�sket��n SDL:n hoitaa viestit, jolloin sen tieto napeista p�ivittyy.
	SDL_PumpEvents();

	// Tarkistetaan pyydetty nappi.
	Uint8 *napit = SDL_GetKeyState(0);

	if(napit == NULL)
	{
		return false;
	}

	switch (n) {
		case NAPPI_VASEN: return napit[SDLK_LEFT];
		case NAPPI_OIKEA: return napit[SDLK_RIGHT];
		case NAPPI_ENTER: return napit[SDLK_RETURN];
		case NAPPI_ESCAPE: return napit[SDLK_ESCAPE];
		case NAPPI_F5: return napit[SDLK_F5];
		case NAPPI_F7: return napit[SDLK_F7];
		case NAPPI_F8: return napit[SDLK_F8];
		case NAPPI_I: return napit[SDLK_i];
		default: return false;
	}
}

// Tyhjent�� sy�tepuskurin.
void Ohjelma::tyhjenna_syote() {
	SDL_Event e;
	while (SDL_PollEvent(&e));
}

apuvalineet::piste Ohjelma::anna_hiiri() {
	int x, y;

	SDL_GetMouseState(&x, &y);
	apuvalineet::piste tmp;

	tmp.x = x;
	tmp.y = y;

	return tmp;
}

bool Ohjelma::lue_hiiri() {
	SDL_PumpEvents();

	if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) {
		return true;
	}

	return false;
}

std::string Versio::anna_versio() {
	const int paa = 1;
	const int kehitys = 0;
	const int rakennus = 2;

	return apuvalineet::tekstiksi(paa) + "." + apuvalineet::tekstiksi(kehitys) + "." + apuvalineet::tekstiksi(rakennus);
}
