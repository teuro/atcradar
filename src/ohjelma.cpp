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

	// Alustetaan SDL tai heitetään virhe.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(SDL_GetError());
	}

	SDL_EnableUNICODE(SDL_ENABLE);

	// Nollataan sekuntilaskuri.
	sekunnit(true);

	std::srand(std::time(NULL));
}

// Lopetusfunktio.
void Ohjelma::loppu() {
	std::clog << "ohjelma::loppu()" << std::endl;

	// Suljetaan SDL.
	SDL_Quit();
}

// Kertoo nollauksesta kuluneiden sekuntien määrän.
double Ohjelma::sekunnit(bool nollaa) {
	Uint32 nyt = SDL_GetTicks();
	if (nollaa) {
		alku_aika = nyt;
	}
	return (nyt - alku_aika) / 1000.0;
}

// Odottaa lyhyen ajan.
void Ohjelma::odota(unsigned int ms) {
	SDL_Delay(ms);
}

IOhjelma::nappi SDKKeyToNappi(SDLKey key)
{
	switch (key) {
	case SDLK_ESCAPE: return IOhjelma::NAPPI_ESCAPE;
	case SDLK_RETURN: return IOhjelma::NAPPI_ENTER;
	case SDLK_LEFT: return IOhjelma::NAPPI_VASEN;
	case SDLK_RIGHT: return IOhjelma::NAPPI_OIKEA;
	case SDLK_DOWN: return IOhjelma::NAPPI_ALAS;
	case SDLK_UP: return IOhjelma::NAPPI_YLOS;
	default: return IOhjelma::NAPPI_MUU;
	}
}

void Ohjelma::process_keyboard(bool wait = false)
{
	SDL_Event event;
	while (SDL_PollEvent(&event) || (wait && SDL_WaitEvent(&event))) {
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.unicode == (Uint16)' ' ||
				((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9')) ||
				((event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Ä')) ||
				((event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z')))
			{
				std::clog << "merkki" << std::endl;
				if (syote.length() <= 16)
				{
					//Append the character
					syote += (char)event.key.keysym.unicode;
				}
			}
			else if (event.key.keysym.sym == SDLK_BACKSPACE)
			{
				std::clog << "backspace" << std::endl;
				if (syote.length() != 0)
				{
					//Remove a character from the end
					syote.erase(syote.length() - 1);
				}
			}
			else {
				nappi nappi = SDKKeyToNappi(event.key.keysym.sym);
				std::clog << "muu " << nappi << std::endl;
				keyboard_buffer.push_back(nappi);
				break;
			}
		}
	}
}

Ohjelma::nappi Ohjelma::lue_nappi() {
	process_keyboard(false);

	if (!keyboard_buffer.empty())
	{
		Ohjelma::nappi nappi = *keyboard_buffer.begin();
		keyboard_buffer.pop_front();
		return nappi;
	}
	else {
		return nappi::NAPPI_MUU;
	}
}

// Lukee seuraavan napinpainalluksen.
Ohjelma::nappi Ohjelma::odota_nappi() {
	// Odotellaan, kunnes tulee napinpainallus.
	process_keyboard(true);
	Ohjelma::nappi nappi = *keyboard_buffer.begin();
	keyboard_buffer.pop_front();
	return nappi;
}

std::string Ohjelma::lue_syote() {
	process_keyboard();
/*	//peli::syote = this->str;
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
				((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9')) ||
				((event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Ä')) ||
				((event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z')))
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
	}*/
	return syote;
}

// Kertoo napin nykytilan.
bool Ohjelma::lue_nappi(nappi n) {
	process_keyboard();

	if (!keyboard_buffer.empty() && *keyboard_buffer.begin() == n) {
		keyboard_buffer.pop_front();
		return true;
	}

	return false;
	// Käsketään SDL:n hoitaa viestit, jolloin sen tieto napeista päivittyy.
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

// Tyhjentää syötepuskurin.
void Ohjelma::tyhjenna_syote() {
	process_keyboard();
	keyboard_buffer.clear();
	syote = "";
	return;
//	SDL_Event e;
// while (SDL_PollEvent(&e));
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
