// ohjelma.cpp
#include "ohjelma.hpp"
#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

namespace ohjelma {
	// Staattisia, siis vain tämän tiedoston käyttöön.
	static SDL_Surface *ruutu;
	static void piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta = false);
	static void piirra_koneet();
	static void piirra_navipisteet();
	static void piirra_lentokentta();
	static void piirra_tilanne();
	static int fontin_koko = 12;
	static void piirra_ohje(std::string ohje);
	static void piirra_virhe(std::string virheteksti);
	static void piirra_metar();
	static void piirra_odottavat();
	std::map <std::string, double> asetukset;

	namespace kuvat {
		// Funktio kuvan lataukseen ja virheen heittämiseen.
		static SDL_Surface *lataa(const char *nimi, bool lapinakyva);

		// Kuvat.
		static SDL_Surface *tausta_valikko, *tausta_peli;
		static SDL_Surface *valikko_peli, *valikko_peli_valittu;
		static SDL_Surface *valikko_lopetus, *valikko_lopetus_valittu;
		static SDL_Surface *tilanne, *tilasto;
		static SDL_Surface *kirjoitus, *ohje, *virhe, *nimi, *atis;
		static SDL_Surface *tekstipinta, *metar, *odottavat;
	}

	static void kirjoita_tekstia(SDL_Surface* tekstipinta, std::string teksti, int x, int y);
	static TTF_Font* fontti;
	static SDL_Color vari = {50, 50, 50};
}

// Alustusfunktio.
void ohjelma::alku() {
	std::clog << "ohjelma::alku()" << std::endl;
	lataa_asetukset("asetukset.ini");

	// Alustetaan SDL tai heitetään virhe.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::runtime_error(SDL_GetError());
	}
	// Avataan ikkuna tai heitetään virhe.
	ruutu = SDL_SetVideoMode(anna_asetus("ruutu_leveys"), anna_asetus("ruutu_korkeus"), 32, SDL_DOUBLEBUF);

	if (!ruutu) {
		throw std::runtime_error(SDL_GetError());
	}

	// Asetetaan otsikko.
	SDL_WM_SetCaption("ATC-radar", "ATC-radar");

	// Nollataan sekuntilaskuri.
	sekunnit(true);

	kuvat::tausta_peli = kuvat::lataa("kuvat/tausta_peli.bmp", false);
	kuvat::tausta_valikko = kuvat::lataa("kuvat/tausta_valikko.bmp", false);
	kuvat::valikko_peli = kuvat::lataa("kuvat/valikko_peli.bmp", false);
	kuvat::valikko_peli_valittu = kuvat::lataa("kuvat/valikko_peli_valittu.bmp", false);
	kuvat::valikko_lopetus = kuvat::lataa("kuvat/valikko_lopetus.bmp", false);
	kuvat::valikko_lopetus_valittu = kuvat::lataa("kuvat/valikko_lopetus_valittu.bmp", false);

	if (TTF_Init() != 0) {
		throw std::runtime_error(TTF_GetError());
	}

	fontti = TTF_OpenFont("fontit/Arial.ttf", fontin_koko);
	SDL_EnableUNICODE(SDL_ENABLE);
}

// Lopetusfunktio.
void ohjelma::loppu() {
	std::clog << "ohjelma::loppu()" << std::endl;

	SDL_FreeSurface(kuvat::tausta_peli);
	SDL_FreeSurface(kuvat::valikko_lopetus);
	SDL_FreeSurface(kuvat::valikko_lopetus_valittu);
	SDL_FreeSurface(kuvat::valikko_peli);
	SDL_FreeSurface(kuvat::valikko_peli_valittu);

	// Suljetaan SDL.
	SDL_Quit();
}

// Kertoo nollauksesta kuluneiden sekuntien määrän.
float ohjelma::sekunnit(bool nollaa) {
	static Uint32 alku;
	Uint32 nyt = SDL_GetTicks();
	if (nollaa) {
		alku = nyt;
	}
	return (nyt - alku) / 1000.0f;
}

// Odottaa lyhyen ajan.
void ohjelma::odota(double ms) {
	SDL_Delay(ms);
}

// Lukee seuraavan napinpainalluksen.
ohjelma::nappi ohjelma::odota_nappi() {
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
bool ohjelma::lue_nappi(nappi n) {
	// Käsketään SDL:n hoitaa viestit, jolloin sen tieto napeista päivittyy.
	SDL_PumpEvents();

	// Tarkistetaan pyydetty nappi.
	Uint8 *napit = SDL_GetKeyState(0);

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
void ohjelma::tyhjenna_syote() {
	SDL_Event e;
	while (SDL_PollEvent(&e));
}

// Lataa kuvan ja optimoi sen piirtoa varten.
static SDL_Surface *ohjelma::kuvat::lataa(const char *nimi, bool lapinakyva) {
	// Jos lataus onnistuu...
	if (SDL_Surface *tmp = SDL_LoadBMP(nimi)) {
		// Asetetaan läpinäkyvä väri (magenta eli pinkki).
		if (lapinakyva) {
			SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(tmp->format, 255, 0, 255));
		}
		// Yritetään optimoida.
		if (SDL_Surface *opti = SDL_DisplayFormat(tmp)) {
			// Tuhotaan alkuperäinen ja palautetaan optimoitu.
			SDL_FreeSurface(tmp);
			tmp = opti;
		}
		// Palautetaan kuva.
		return tmp;
	}
	// Muuten heitetään virhe.
	throw std::runtime_error(SDL_GetError());
}

// Piirtää yhden kuvan.
static void ohjelma::piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta) {
	SDL_Rect r = {x, y};

	if (keskikohta) {
		r.x -= kuva->w / 2;
		r.y -= kuva->h / 2;
	}

	SDL_BlitSurface(kuva, 0, ruutu, &r);
}

// Piirtää valikon.
void ohjelma::piirra_valikko(int pelin_tulos, valikko::valinta valittu) {
	std::clog << "ohjelma::piirra_valikko(tulos, valittu)" << std::endl;

	// Valitaan oikeat kuvat.
	SDL_Surface *kuva_peli = kuvat::valikko_peli;
	SDL_Surface *kuva_lopetus = kuvat::valikko_lopetus;

	switch (valittu) {
		case valikko::PELI:
			kuva_peli = kuvat::valikko_peli_valittu;
			break;
		case valikko::LOPETUS:
			kuva_lopetus = kuvat::valikko_lopetus_valittu;
			break;
	}

	int x = 20, y = 16;
	piirra_kuva(kuvat::tausta_valikko, 0, 0);

	piirra_kuva(kuva_peli, x, y);
	piirra_kuva(kuva_lopetus, x, y + kuva_peli->h + 5);

	// Laitetaan piirustukset esille.
	SDL_Flip(ruutu);
}

// Piirtää pelin.
void ohjelma::piirra_peli() {
	piirra_kuva(kuvat::tausta_peli, 0, 0);
	piirra_tilanne();

	piirra_koneet();

	piirra_navipisteet();

	piirra_lentokentta();
	piirra_ohje(peli::ohje);
	piirra_virhe(peli::virheteksti);
	piirra_odottavat();

	piirra_metar();

	SDL_Flip(ruutu);
}

void ohjelma::piirra_koneet() {
	Uint32 vari;

	SDL_Surface* tiedot;

	double lentokorkeus;
	double selvityskorkeus;

	Uint32 ok = 0x000000FF;
	Uint32 ng = 0xFF0000FF;

	for (unsigned int i = 0; i < peli::koneet.size(); ++i) {
		if (peli::koneet[i].odotus == false) {
			apuvalineet::piste loppupiste = apuvalineet::uusi_paikka(peli::koneet[i].paikka, peli::koneet[i].suunta, peli::koneet[i].nopeus * (60.0 / 3600.0));

			if (peli::koneet[i].onko_porrastus) {
				vari = ok;
			} else {
				vari = ng;
			}

			rectangleColor(ruutu, peli::koneet[i].paikka.x-3, peli::koneet[i].paikka.y-3, peli::koneet[i].paikka.x+3, peli::koneet[i].paikka.y+3, vari);
			lineColor(ruutu, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y, loppupiste.x, loppupiste.y, vari);
			circleColor(ruutu, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y, apuvalineet::nm2px(1.5), vari);

			kirjoita_tekstia(tiedot, peli::koneet[i].kutsutunnus, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y);

			lentokorkeus 	= std::floor(peli::koneet[i].korkeus);
			selvityskorkeus = std::floor(peli::koneet[i].selvityskorkeus);

			if (peli::koneet[i].korkeus > peli::atis::siirtokorkeus) {
				lentokorkeus 	= std::floor(peli::koneet[i].korkeus / 100);
			}

			if (peli::koneet[i].selvityskorkeus > peli::atis::siirtokorkeus) {
				selvityskorkeus = std::floor(peli::koneet[i].selvityskorkeus / 100);
			}

			kirjoita_tekstia(tiedot, apuvalineet::tekstiksi(lentokorkeus) + " / ", peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + fontin_koko + 3);
			kirjoita_tekstia(tiedot, apuvalineet::tekstiksi(selvityskorkeus), peli::koneet[i].paikka.x + 40, peli::koneet[i].paikka.y + fontin_koko + 3);

			if (peli::koneet[i].valittu) {
				kirjoita_tekstia(tiedot, apuvalineet::tekstiksi(peli::koneet[i].nopeus) + " / ", peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + (2 * fontin_koko) + 3);
				kirjoita_tekstia(tiedot, apuvalineet::tekstiksi(peli::koneet[i].selvitysnopeus), peli::koneet[i].paikka.x + 40, peli::koneet[i].paikka.y + (2 * fontin_koko) + 3);
				kirjoita_tekstia(tiedot, apuvalineet::tekstiksi(peli::koneet[i].suunta) + " / ", peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + (3 * fontin_koko) + 3);
				kirjoita_tekstia(tiedot, apuvalineet::tekstiksi(peli::koneet[i].selvityssuunta), peli::koneet[i].paikka.x + 40, peli::koneet[i].paikka.y + (3 * fontin_koko) + 3);

				if (peli::koneet[i].tyyppi == peli::LAHTEVA) {
					kirjoita_tekstia(tiedot, peli::koneet[i].ulosmenopiste.nimi, peli::koneet[i].paikka.x, peli::koneet[i].paikka.y + (4 * fontin_koko) + 3);
				}
			}
		}
	}
}

void ohjelma::piirra_navipisteet() {
	SDL_Surface* nimi;

	for (unsigned int i = 0; i < peli::navipisteet.size(); ++i) {
		apuvalineet::piste tmp = peli::navipisteet[i].paikka;
		kirjoita_tekstia(nimi, peli::navipisteet[i].nimi, tmp.x, tmp.y);
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
		//std::clog << asetus_nimi << " => " << ohjelma::asetukset[asetus_nimi] << std::endl;
	}

	//std::clog << asetukset.size() << std::endl;

	sisaan.close();
}

double ohjelma::anna_asetus(std::string asetus) {
	std::map <std::string, double>::iterator onko;

	onko = ohjelma::asetukset.find(asetus);

	if (onko == ohjelma::asetukset.end()) {
		throw std::logic_error("Asetusta " + asetus + " ei ole rivi");
	}

	return ohjelma::asetukset[asetus];
}

bool ohjelma::onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade) {
	double x = std::abs(a.x - b.x);
	double y = std::abs(a.y - b.y);

	double valimatka = std::sqrt((x*x) + (y*y));

	return valimatka < (2 * apuvalineet::nm2px(sade));
}

apuvalineet::piste ohjelma::anna_hiiri() {
	int x, y;

	SDL_GetMouseState(&x, &y);
	apuvalineet::piste tmp;

	tmp.x = x;
	tmp.y = y;

	return tmp;
}

bool ohjelma::lue_hiiri() {
	SDL_PumpEvents();

	if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) {
		return true;
	}

	return false;
}

void ohjelma::piirra_lentokentta() {
	for (unsigned int i = 0; i < peli::kentta.kiitotiet.size(); ++i) {
		lineColor(ruutu, peli::kentta.kiitotiet[i].alkupiste.x, peli::kentta.kiitotiet[i].alkupiste.y, peli::kentta.kiitotiet[i].loppupiste.x, peli::kentta.kiitotiet[i].loppupiste.y, 0x223344FF);
		kirjoita_tekstia(kuvat::nimi, peli::kentta.kiitotiet[i].nimi, peli::kentta.kiitotiet[i].alkupiste.x, peli::kentta.kiitotiet[i].alkupiste.y);

		circleColor(ruutu, peli::kentta.kiitotiet[i].lahestymispiste.x, peli::kentta.kiitotiet[i].lahestymispiste.y, apuvalineet::nm2px(0.5), 0xAAAAAAFF);
	}
}

void ohjelma::kirjoita_tekstia(SDL_Surface* tekstipinta, std::string teksti, int x, int y) {
	tekstipinta = TTF_RenderText_Solid(fontti, teksti.c_str(), vari);

	if (!tekstipinta) {
		throw std::runtime_error(SDL_GetError());
	}

	piirra_kuva(tekstipinta, x, y, false);
	SDL_FreeSurface(tekstipinta);
}

void ohjelma::kirjoita_tekstia(std::string teksti, int x, int y) {
	if (teksti.length() > 0) {
		kuvat::tekstipinta = TTF_RenderText_Solid(fontti, teksti.c_str(), vari);

		if (!kuvat::tekstipinta) {
			throw std::runtime_error(SDL_GetError());
		}
		piirra_kuva(kuvat::tekstipinta, x, y, false);
		SDL_Flip(ruutu);
	}
}

void ohjelma::piirra_tilanne() {
	std::string teksti = "Käsitellyt " + apuvalineet::tekstiksi(peli::kasitellyt) + std::string("/") + apuvalineet::tekstiksi(anna_asetus("vaadittavat_kasitellyt"));
	kirjoita_tekstia(kuvat::tilanne, teksti, ohjelma::anna_asetus("ruutu_leveys") - (teksti.length() * (fontin_koko / 2)), 20);

	teksti = "porrastusvirheet " + apuvalineet::tekstiksi(peli::porrastusvirheet) + std::string("/") + apuvalineet::tekstiksi(anna_asetus("maks_porrastusvirhe"));
	kirjoita_tekstia(kuvat::tilanne, teksti, ohjelma::anna_asetus("ruutu_leveys") - (teksti.length() * (fontin_koko / 2)), 40);

	teksti =  "muut virheet " + apuvalineet::tekstiksi(peli::muut_virheet);
	kirjoita_tekstia(kuvat::tilanne, teksti, ohjelma::anna_asetus("ruutu_leveys") - (teksti.length() * (fontin_koko / 2)), 60);
}

void ohjelma::piirra_ohje(std::string ohje) {
	kirjoita_tekstia(kuvat::ohje, ohje.c_str(), anna_asetus("ruutu_leveys") - (ohje.length() * (fontin_koko / 2)), 80);
}

void ohjelma::piirra_tilasto() {
	std::clog << "ohjelma::piirra_tilasto()" << std::endl;
	piirra_kuva(kuvat::tausta_valikko, 0, 0);
	int y = 30;
	int x = 200;
	kirjoita_tekstia(kuvat::tilasto, "kutsutunnus", x, y-15);
	kirjoita_tekstia(kuvat::tilasto, "alueelle", x+100, y-15);
	kirjoita_tekstia(kuvat::tilasto, "pois", x+200, y-15);
	kirjoita_tekstia(kuvat::tilasto, "alueella", x+300, y-15);
	kirjoita_tekstia(kuvat::tilasto, "selvitykset", x+400, y-15);

	for (unsigned int i = 0; i < peli::ajat.size(); ++i) {
		kirjoita_tekstia(kuvat::tilasto, peli::ajat[i].tunnus, x, y);
		kirjoita_tekstia(kuvat::tilasto, apuvalineet::tekstiksi(peli::ajat[i].sisaan).c_str(), x+100, y);
		kirjoita_tekstia(kuvat::tilasto, apuvalineet::tekstiksi(peli::ajat[i].pois).c_str(), x+200, y);
		kirjoita_tekstia(kuvat::tilasto, apuvalineet::tekstiksi(peli::ajat[i].pois - peli::ajat[i].sisaan).c_str(), x+300, y);
		kirjoita_tekstia(kuvat::tilasto, apuvalineet::tekstiksi(peli::ajat[i].selvitykset).c_str(), x+400, y);

		y += 15;
	}

	SDL_Flip(ruutu);
}

void ohjelma::piirra_kirjoitus(std::string teksti) {
	if (teksti.length() > 0) {
		kirjoita_tekstia(kuvat::kirjoitus, teksti, 500, 20);
	}

	SDL_Flip(ruutu);
}

void ohjelma::piirra_virhe(std::string virhe) {
	kirjoita_tekstia(kuvat::virhe, virhe, 550, 100);
}

void ohjelma::piirra_atis(int toiminto) {
	piirra_kuva(kuvat::tausta_valikko, 0, 0);

	switch (toiminto) {
		case peli::LAHTO:
			kirjoita_tekstia(kuvat::atis, "Valitse lähtökiitotie", 400, 70);
			break;
		case peli::LASKU:
			kirjoita_tekstia(kuvat::atis, "Valitse laskukiitotie", 400, 70);
			break;
		case peli::SIIRTOPINTA:
			kirjoita_tekstia(kuvat::atis, "Valitse siirtopinta", 400, 70);
			break;
	}

	int y = 30;
	for (unsigned int i = 0; i < peli::kentta.kiitotiet.size(); ++i) {
		kirjoita_tekstia(kuvat::atis, peli::kentta.kiitotiet[i].nimi, 400, y);
		y += 20;
	}

	piirra_metar();
	piirra_ohje(peli::ohje);

	kirjoita_tekstia(kuvat::atis, "Anna lähtö- ja laskukiitotie, sekä siirtopinta", 30, 30);
	kirjoita_tekstia(kuvat::atis, "Lähtökiitotie: " + apuvalineet::tekstiksi(peli::atis::lahto), 30, 50);
	kirjoita_tekstia(kuvat::atis, "Laskukiitotie: " + apuvalineet::tekstiksi(peli::atis::lasku), 30, 70);
	kirjoita_tekstia(kuvat::atis, "Siirtopinta: " + apuvalineet::tekstiksi(peli::atis::siirtopinta), 30, 90);

	SDL_Flip(ruutu);
}

void ohjelma::piirra_metar() {
	std::string tuuli = apuvalineet::tekstiksi(peli::metar::tuuli);
	std::string voimakkuus = apuvalineet::tekstiksi(peli::metar::voimakkuus);

	if (tuuli.length() == 1) {
		tuuli = "00" + tuuli;
	} else if (tuuli.length() == 2) {
		tuuli = "0" + tuuli;
	}

	if (voimakkuus.length() == 1) {
		voimakkuus = "0" + voimakkuus;
	}

	kirjoita_tekstia(kuvat::metar, tuuli + "/" + voimakkuus + " " + apuvalineet::tekstiksi(peli::metar::paine), 30, 10);
}

static void ohjelma::piirra_odottavat() {
	int y = 100;
	for (unsigned int i = 0; i < peli::odottavat.size(); ++i) {
		kirjoita_tekstia(kuvat::odottavat, peli::odottavat[i].kutsutunnus, anna_asetus("ruutu_leveys") - 100, y);

		y += fontin_koko + 5;
	}
}

