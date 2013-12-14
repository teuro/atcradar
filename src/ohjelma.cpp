// ohjelma.cpp
#include "asetukset.h"
#include "ohjelma.hpp"
#include "kuvavarasto.hpp"
#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

void Ohjelma::alku() {
    // Remove?
	// Alustetaan SDL tai heitet‰‰n virhe.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
	SDL_EnableUNICODE(SDL_ENABLE);
}

// Lopetusfunktio.
void Ohjelma::loppu() {
	std::clog << "ohjelma::loppu()" << std::endl;

	// Suljetaan SDL.
	SDL_Quit();
}

// Kertoo nollauksesta kuluneiden sekuntien m‰‰r‰n.
float Ohjelma::sekunnit(bool nollaa) {
	static Uint32 alku;
	Uint32 nyt = SDL_GetTicks();
	if (nollaa) {
		alku = nyt;
	}
	return (nyt - alku) / 1000.0f;
}

// Odottaa lyhyen ajan.
void Ohjelma::odota(double ms) {
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
			default: return NAPPI_MUU;
		}
	}
	// Jokin meni pieleen!
	throw std::runtime_error(SDL_GetError());
}

// Kertoo napin nykytilan.
bool Ohjelma::lue_nappi(nappi n) {
	// K‰sket‰‰n SDL:n hoitaa viestit, jolloin sen tieto napeista p‰ivittyy.
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

// Tyhjent‰‰ syˆtepuskurin.
void Ohjelma::tyhjenna_syote() {
	SDL_Event e;
	while (SDL_PollEvent(&e));
}

<<<<<<< HEAD
// Piirt‰‰ yhden kuvan.
static void ohjelma::piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta) {
	SDL_Rect r = {(Sint16)x, (Sint16)y};

	if (keskikohta) {
		r.x -= kuva->w / 2;
		r.y -= kuva->h / 2;
	}

	SDL_BlitSurface(kuva, 0, ruutu, &r);
}

// Piirt‰‰ valikon.
void ohjelma::piirra_valikko(int pelin_tulos, valikko::valinta valittu) {
	std::clog << "ohjelma::piirra_valikko(tulos, valittu)" << std::endl;

	// Valitaan oikeat kuvat.
	SDL_Surface *kuva_peli 		= image_cache::common().get("kuvat/valikko_peli.png");
	SDL_Surface *kuva_lopetus 	= image_cache::common().get("kuvat/valikko_lopetus.png");

	switch (valittu) {
		case valikko::PELI:
			kuva_peli = image_cache::common().get("kuvat/valikko_peli_valittu.png");
			break;
		case valikko::LOPETUS:
			kuva_lopetus = image_cache::common().get("kuvat/valikko_lopetus_valittu.png");
			break;
	}

	int x = 20, y = 16;
	piirra_kuva(image_cache::common().get("kuvat/tausta_valikko.png"), 0, 0);

	piirra_kuva(kuva_peli, x, y);
	piirra_kuva(kuva_lopetus, x, y + kuva_peli->h + 5);

	// Laitetaan piirustukset esille.
	SDL_Flip(ruutu);
}

// Piirt‰‰ pelin.
void ohjelma::piirra_peli() {
	piirra_kuva(image_cache::common().get("kuvat/tausta_peli.png"), 0, 0);
	piirra_tilanne();

	piirra_koneet();

	piirra_navipisteet();

	piirra_lentokentta();
	piirra_ohje(peli::ohje);
	piirra_odottavat();

	kirjoita_tekstia(peli::syote, 50, 70);

	piirra_metar();

	SDL_Flip(ruutu);
}

void ohjelma::piirra_koneet() {
	Uint32 vari;

	double lentokorkeus;
	double selvityskorkeus;

	Uint32 ok = 0x000000FF;
	Uint32 ng = 0xFF0000FF;
	int t_y = 90;
	for (unsigned int i = 0; i < peli::koneet.size(); ++i) {
		if (peli::koneet[i].odotus == false) {
			apuvalineet::piste loppupiste = apuvalineet::uusi_paikka(peli::koneet[i].paikka, peli::koneet[i].suunta, peli::koneet[i].nopeus * (60.0 / 3600.0));
			apuvalineet::piste hiiri = anna_hiiri();

			if (peli::koneet[i].onko_porrastus) {
				vari = ok;
			} else {
				vari = ng;
			}

			rectangleColor(ruutu, peli::koneet[i].paikka.x-3, peli::koneet[i].paikka.y-3, peli::koneet[i].paikka.x+3, peli::koneet[i].paikka.y+3, vari);
			lineColor(ruutu, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y, loppupiste.x, loppupiste.y, vari);
			circleColor(ruutu, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y, apuvalineet::nm2px(1.5), vari);

			kirjoita_tekstia(peli::koneet[i].kutsutunnus, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y);
			
			kirjoita_tekstia(peli::koneet[i].kutsutunnus, 30, t_y); 
			
			if (peli::koneet[i].reitti.size() == 0) {
				kirjoita_tekstia(apuvalineet::tekstiksi(peli::koneet[i].selvityssuunta), 100, t_y);
			} else {
				kirjoita_tekstia(peli::koneet[i].kohde.nimi, 100, t_y);
			}
			
			kirjoita_tekstia(apuvalineet::tekstiksi(peli::koneet[i].selvityskorkeus), 160, t_y);
			kirjoita_tekstia(apuvalineet::tekstiksi(peli::koneet[i].selvitysnopeus), 210, t_y);
			t_y += 20;

			if (peli::koneet[i].tyyppi == peli::SAAPUVA) {
				//std::clog << "Tulee" << std::endl;
				if ((peli::koneet[i].korkeus / 100) < peli::atis::siirtopinta) {
					lentokorkeus = std::floor(peli::koneet[i].korkeus);
				} else {
					lentokorkeus = std::floor(peli::koneet[i].korkeus / 100);
				}

				if ((peli::koneet[i].selvityskorkeus / 100) < peli::atis::siirtopinta) {
					selvityskorkeus = std::floor(peli::koneet[i].selvityskorkeus);
				} else {
					selvityskorkeus = std::floor(peli::koneet[i].selvityskorkeus / 100);
				}

			} else {
				//std::clog << "Menee" << std::endl;
				if (peli::koneet[i].korkeus < peli::atis::siirtokorkeus) {
					lentokorkeus = std::floor(peli::koneet[i].korkeus);
				} else {
					lentokorkeus = std::floor(peli::koneet[i].korkeus / 100);
				}

				if (peli::koneet[i].selvityskorkeus < peli::atis::siirtokorkeus) {
					selvityskorkeus = std::floor(peli::koneet[i].selvityskorkeus);
				} else {
					selvityskorkeus = std::floor(peli::koneet[i].selvityskorkeus / 100);
				}
			}

			kirjoita_tekstia(apuvalineet::tekstiksi(lentokorkeus) + " / " + apuvalineet::tekstiksi(selvityskorkeus), peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + fontin_koko + 3);

			if (peli::koneet[i].valittu) {
				kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(peli::koneet[i].nopeus)) + " / " + apuvalineet::tekstiksi(peli::koneet[i].selvitysnopeus), peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + (2 * fontin_koko) + 3);
				kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(peli::koneet[i].suunta)) + " / " + apuvalineet::tekstiksi(peli::koneet[i].selvityssuunta), peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + (3 * fontin_koko) + 3);

				if (peli::koneet[i].tyyppi == peli::LAHTEVA) {
					kirjoita_tekstia(peli::koneet[i].ulosmenopiste.nimi, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + (4 * fontin_koko) + 3);
				}

				switch (peli::toiminto) {
					case peli::SUUNTA:
						kirjoita_tekstia("Anna suunta 0-360 tai navipisteen nimi", 50, 50);
						break;
					case peli::NOPEUS:
						kirjoita_tekstia("Anna nopeus lukuna", 50, 50);
						break;
					case peli::KORKEUS:
						kirjoita_tekstia("Anna korkeus numeroina", 50, 50);
						break;
				}

				lineColor(ruutu, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y, hiiri.x, hiiri.y, vari);
				apuvalineet::vektori vek = apuvalineet::suunta_vektori(peli::koneet[i].paikka, hiiri);

				int x = std::abs(peli::koneet[i].paikka.x + hiiri.x) / 2;
				int y = std::abs(peli::koneet[i].paikka.y + hiiri.y) / 2;

				kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(vek.pituus)) + " / " + apuvalineet::tekstiksi(std::floor(vek.suunta)), x, y);
			}
		}
	}
}

void ohjelma::piirra_navipisteet() {
	for (unsigned int i = 0; i < peli::navipisteet.size(); ++i) {
		apuvalineet::piste tmp = peli::navipisteet[i].paikka;
		kirjoita_tekstia(peli::navipisteet[i].nimi, tmp.x, tmp.y);
		trigonColor(ruutu, tmp.x-3, tmp.y+3, tmp.x+3, tmp.y+3, tmp.x, tmp.y-3, 456);
	}

	for (unsigned int i = 0; i < peli::sisapisteet.size(); ++i) {
		apuvalineet::piste tmp = peli::sisapisteet[i].paikka;
		kirjoita_tekstia(peli::sisapisteet[i].nimi, tmp.x, tmp.y);
		trigonColor(ruutu, tmp.x-3, tmp.y+3, tmp.x+3, tmp.y+3, tmp.x, tmp.y-3, 456);
	}
}

void ohjelma::lataa_asetukset(std::string nimi) {
	std::clog << "ohjelma::lataa_asetukset(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedosto " + nimi + " ei aukea tai puuttuu");
	}

	std::string asetus_nimi;
	double asetus_arvo;

	while (sisaan >> asetus_nimi >> asetus_arvo) {
		ohjelma::asetukset[asetus_nimi] = asetus_arvo;
	}

	sisaan.close();
}

double ohjelma::anna_asetus(std::string asetus) {
	std::map <std::string, double>::iterator onko;

	onko = asetukset.find(asetus);

	if (onko == ohjelma::asetukset.end()) {
		throw std::logic_error("Asetusta " + asetus + " ei ole");
	}

	return asetukset[asetus];
}

bool ohjelma::onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade) {
=======
bool Ohjelma::onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade) {
>>>>>>> 666aafec758181748575d76a3a5154ba0e3f75b9
	double x = std::abs(a.x - b.x);
	double y = std::abs(a.y - b.y);

	double valimatka = std::sqrt((x*x) + (y*y));

	return valimatka < (2 * apuvalineet::nm2px(sade));
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
	return apuvalineet::tekstiksi(paa) + "." + apuvalineet::tekstiksi(kehitys) + "." + apuvalineet::tekstiksi(rakennus);
}
