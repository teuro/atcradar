#include "asetukset.h"
#include "peliview.hpp"
#include "ohjelma.hpp"
#include "kuvavarasto.hpp"
#include <iostream>
#include <stdexcept>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

// Alustusfunktio.
void PeliView::alku() {
	std::clog << "PeliView::alku()" << std::endl;
	// Avataan ikkuna tai heitet‰‰n virhe.
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

// Piirt‰‰ yhden kuvan.
void PeliView::piirra_kuva(SDL_Surface *kuva, int x, int y, bool keskikohta) {
	SDL_Rect r = { (Sint16)x, (Sint16)y };

	if (keskikohta) {
		r.x -= kuva->w / 2;
		r.y -= kuva->h / 2;
	}

	SDL_BlitSurface(kuva, 0, ruutu, &r);
}

// Piirt‰‰ valikon.
void PeliView::piirra_valikko(valikko::valinta valittu) {
	std::clog << "ohjelma::piirra_valikko(tulos, valittu)" << std::endl;

	// Valitaan oikeat kuvat.
	SDL_Surface *kuva_peli = image_cache::common().get("kuvat/valikko_peli.png");
	SDL_Surface *kuva_lopetus = image_cache::common().get("kuvat/valikko_lopetus.png");

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
void PeliView::piirra_peli() {
	piirra_kuva(image_cache::common().get("kuvat/tausta_peli.png"), 0, 0);
	piirra_tilanne();

	piirra_koneet();

	piirra_navipisteet();

	piirra_lentokentta();
	piirra_ohje(peli.ohje);
	piirra_odottavat();

	kirjoita_tekstia(peli.syote, 50, 70);

	piirra_metar();

	SDL_Flip(ruutu);
}

void PeliView::piirra_koneet() {
	Uint32 vari;

	double lentokorkeus;
	double selvityskorkeus;

	Uint32 ok = 0x000000FF;
	Uint32 ng = 0xFF0000FF;
	
	int listauskorkeus = 90;
	
	for (unsigned int i = 0; i < peli.koneet.size(); ++i) {
		if (peli.koneet[i].odotus == false) {
			apuvalineet::piste loppupiste = apuvalineet::uusi_paikka(peli.koneet[i].paikka, peli.koneet[i].suunta, peli.koneet[i].nopeus * (60.0 / 3600.0));
			apuvalineet::piste hiiri = ohjelma.anna_hiiri();

			if (peli.koneet[i].onko_porrastus) {
				vari = ok;
			}
			else {
				vari = ng;
			}

			rectangleColor(ruutu, peli.koneet[i].paikka.x - 3, peli.koneet[i].paikka.y - 3, peli.koneet[i].paikka.x + 3, peli.koneet[i].paikka.y + 3, vari);
			lineColor(ruutu, peli.koneet[i].paikka.x, peli.koneet[i].paikka.y, loppupiste.x, loppupiste.y, vari);
			circleColor(ruutu, peli.koneet[i].paikka.x, peli.koneet[i].paikka.y, apuvalineet::nm2px(1.5), vari);

			kirjoita_tekstia(peli.koneet[i].kutsutunnus, peli.koneet[i].paikka.x, peli.koneet[i].paikka.y);
			
			kirjoita_tekstia(peli.koneet[i].kutsutunnus, 30, listauskorkeus); 
			
			if (peli.koneet[i].reitti.size() == 0) {
				if (peli.koneet[i].lahestymisselvitys) {
					kirjoita_tekstia("ILS", 100, listauskorkeus);
				} else if (peli.koneet[i].oikotie) {
					kirjoita_tekstia(peli.koneet[i].ulosmenopiste.nimi, 100, listauskorkeus);
				}
					
				if (!peli.koneet[i].lahestymisselvitys && !peli.koneet[i].oikotie) {
					kirjoita_tekstia(apuvalineet::tekstiksi(peli.koneet[i].selvityssuunta), 100, listauskorkeus);
				}
			} else {
				kirjoita_tekstia(peli.koneet[i].kohde.nimi, 100, listauskorkeus); 
			}
			
			kirjoita_tekstia(apuvalineet::tekstiksi(peli.koneet[i].selvityskorkeus), 160, listauskorkeus);
			kirjoita_tekstia(apuvalineet::tekstiksi(peli.koneet[i].selvitysnopeus), 210, listauskorkeus);
			listauskorkeus += 20;


			if (peli.koneet[i].tyyppi == Peli::SAAPUVA) {
				//std::clog << "Tulee" << std::endl;
				if ((peli.koneet[i].korkeus / 100) < peli.atis.siirtopinta) {
					lentokorkeus = std::floor(peli.koneet[i].korkeus);
				}
				else {
					lentokorkeus = std::floor(peli.koneet[i].korkeus / 100);
				}

				if ((peli.koneet[i].selvityskorkeus / 100) < peli.atis.siirtopinta) {
					selvityskorkeus = std::floor(peli.koneet[i].selvityskorkeus);
				}
				else {
					selvityskorkeus = std::floor(peli.koneet[i].selvityskorkeus / 100);
				}

			}
			else {
				//std::clog << "Menee" << std::endl;
				if (peli.koneet[i].korkeus < peli.atis.siirtokorkeus) {
					lentokorkeus = std::floor(peli.koneet[i].korkeus);
				}
				else {
					lentokorkeus = std::floor(peli.koneet[i].korkeus / 100);
				}

				if (peli.koneet[i].selvityskorkeus < peli.atis.siirtokorkeus) {
					selvityskorkeus = std::floor(peli.koneet[i].selvityskorkeus);
				}
				else {
					selvityskorkeus = std::floor(peli.koneet[i].selvityskorkeus / 100);
				}
			}

			kirjoita_tekstia(apuvalineet::tekstiksi(lentokorkeus) + " / " + apuvalineet::tekstiksi(selvityskorkeus), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + fontin_koko + 3);

			if (peli.koneet[i].valittu) {
				kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(peli.koneet[i].nopeus)) + " / " + apuvalineet::tekstiksi(peli.koneet[i].selvitysnopeus), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + (2 * fontin_koko) + 3);
				kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(peli.koneet[i].suunta)) + " / " + apuvalineet::tekstiksi(peli.koneet[i].selvityssuunta), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + (3 * fontin_koko) + 3);

				if (peli.koneet[i].tyyppi == Peli::LAHTEVA) {
					kirjoita_tekstia(peli.koneet[i].ulosmenopiste.nimi, peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + (4 * fontin_koko) + 3);
				}

				switch (peli.toiminto) {
				case Peli::SUUNTA:
					kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_OHJE_SUUNTA), 50, 50);
					break;
				case Peli::NOPEUS:
					kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_OHJE_NOPEUS), 50, 50);
					break;
				case Peli::KORKEUS:
					kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_OHJE_KORKEUS), 50, 50);
					break;
				}

				lineColor(ruutu, peli.koneet[i].paikka.x, peli.koneet[i].paikka.y, hiiri.x, hiiri.y, vari);
				apuvalineet::vektori vek = apuvalineet::suunta_vektori(peli.koneet[i].paikka, hiiri);

				int x = std::abs(peli.koneet[i].paikka.x + hiiri.x) / 2;
				int y = std::abs(peli.koneet[i].paikka.y + hiiri.y) / 2;

				kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(vek.pituus)) + " / " + apuvalineet::tekstiksi(std::floor(vek.suunta)), x, y);
			}
		}
	}
}

void PeliView::piirra_navipisteet() {
	for (unsigned int i = 0; i < peli.navipisteet.size(); ++i) {
		apuvalineet::piste tmp = peli.navipisteet[i].paikka;
		kirjoita_tekstia(peli.navipisteet[i].nimi, tmp.x, tmp.y);
		trigonColor(ruutu, tmp.x - 3, tmp.y + 3, tmp.x + 3, tmp.y + 3, tmp.x, tmp.y - 3, 456);
	}

	for (unsigned int i = 0; i < peli.sisapisteet.size(); ++i) {
		apuvalineet::piste tmp = peli.sisapisteet[i].paikka;
		kirjoita_tekstia(peli.sisapisteet[i].nimi, tmp.x, tmp.y);
		trigonColor(ruutu, tmp.x - 3, tmp.y + 3, tmp.x + 3, tmp.y + 3, tmp.x, tmp.y - 3, 456);
	}
}

void PeliView::piirra_lentokentta() {
	for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
		lineColor(ruutu, peli.kentta.kiitotiet[i].alkupiste.x, peli.kentta.kiitotiet[i].alkupiste.y, peli.kentta.kiitotiet[i].loppupiste.x, peli.kentta.kiitotiet[i].loppupiste.y, 0x223344FF);
		kirjoita_tekstia(peli.kentta.kiitotiet[i].nimi, peli.kentta.kiitotiet[i].alkupiste.x, peli.kentta.kiitotiet[i].alkupiste.y);

		circleColor(ruutu, peli.kentta.kiitotiet[i].lahestymispiste.x, peli.kentta.kiitotiet[i].lahestymispiste.y, apuvalineet::nm2px(0.5), 0xAAAAAAFF);
	}
}

void PeliView::kirjoita_tekstia(std::string teksti, int x, int y) {
	if (teksti.length() > 0) {
		SDL_Surface* tekstipinta = TTF_RenderText_Solid(fontti, teksti.c_str(), vari);

		if (!tekstipinta) {
			throw std::runtime_error(SDL_GetError());
		}

		piirra_kuva(tekstipinta, x, y, false);
		SDL_FreeSurface(tekstipinta);
	}
}

void PeliView::piirra_tilanne() {
	std::string teksti = "K‰sitellyt " + apuvalineet::tekstiksi(peli.kasitellyt) + std::string("/") + apuvalineet::tekstiksi(asetukset.anna_asetus("vaadittavat_kasitellyt"));
	kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), 20);

	teksti = "porrastusvirheet " + apuvalineet::tekstiksi(peli.porrastusvirheet) + std::string("/") + apuvalineet::tekstiksi(asetukset.anna_asetus("maks_porrastusvirhe"));
	kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), 40);

	teksti = "muut virheet " + apuvalineet::tekstiksi(peli.muut_virheet);
	kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), 60);
}

void PeliView::piirra_ohje(std::string ohje) {
	kirjoita_tekstia(ohje.c_str(), 50, 30);
}

void PeliView::piirra_tilasto() {
	std::clog << "ohjelma::piirra_tilasto()" << std::endl;

	piirra_kuva(image_cache::common().get("kuvat/tausta_tilasto.png"), 0, 0);

	int y = 30;
	int x = 200;

	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_KUTSUTUNNUS), x, y - 15);
	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_ALUEELLE), x + 100, y - 15);
	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_POIS), x + 200, y - 15);
	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_ALUEELLA), x + 300, y - 15);
	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_SELVITYKSET), x + 400, y - 15);
	
	double ka_alueella = 0;
	double ka_alueelta = 0;
	double ka_selvitykset = 0;
	double ka_alueelle = 0;
	
	for (unsigned int i = 0; i < peli.ajat.size(); ++i) {
		double alueella = peli.ajat[i].pois - peli.ajat[i].sisaan;
		ka_alueella += alueella;
		ka_selvitykset += peli.ajat[i].selvitykset;
		if (i > 0) {
			ka_alueelle += peli.ajat[i-1].sisaan;
			ka_alueelta += peli.ajat[i-1].pois;
		}
		
		kirjoita_tekstia(peli.ajat[i].tunnus, x, y);
		kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].sisaan).c_str(), x + 100, y);
		kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].pois).c_str(), x + 200, y);
		kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].pois - peli.ajat[i].sisaan).c_str(), x + 300, y);
		kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].selvitykset).c_str(), x + 400, y);

		y += 15;
	}
	
	kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat.size()), x, y);
	kirjoita_tekstia(apuvalineet::tekstiksi(ka_alueelle / peli.ajat.size()), x + 100, y);
	kirjoita_tekstia(apuvalineet::tekstiksi(ka_alueelta / peli.ajat.size()), x + 200, y);
	kirjoita_tekstia(apuvalineet::tekstiksi(ka_alueella), x + 300, y);
	kirjoita_tekstia(apuvalineet::tekstiksi(ka_selvitykset / peli.ajat.size()), x + 400, y);

	SDL_Flip(ruutu);
}

void PeliView::piirra_atis() {
	piirra_kuva(image_cache::common().get("kuvat/tausta_atis.png"), 0, 0);

	kirjoita_tekstia(peli.ohje, 50, 30);
	kirjoita_tekstia(peli.syote, 50, 50);

	int y = 70;

	for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
		kirjoita_tekstia(peli.kentta.kiitotiet[i].nimi, 400, y);
		y += 20;
	}

	piirra_metar();
	piirra_ohje(peli.ohje);

	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_OHJE_LAHTOKIITOTIE), 50, 90);
	kirjoita_tekstia(apuvalineet::tekstiksi(peli.atis.lahto), 150, 90);
	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_OHJE_LASKUKIITOTIE), 50, 110);
	kirjoita_tekstia(apuvalineet::tekstiksi(peli.atis.lasku), 150, 110);
	kirjoita_tekstia(kieli.anna_teksti(Ohjelma::TEKSTI_OHJE_SIIRTOPINTA), 50, 130);
	kirjoita_tekstia(apuvalineet::tekstiksi(peli.atis.siirtopinta), 150, 130);

	SDL_Flip(ruutu);
}

void PeliView::piirra_metar() {
	std::string tuuli 		= apuvalineet::tekstiksi(peli.metar.tuuli);
	std::string voimakkuus 	= apuvalineet::tekstiksi(peli.metar.voimakkuus);
	std::string paine 		= apuvalineet::tekstiksi(peli.metar.paine);
	std::string nakyvyys 	= apuvalineet::tekstiksi(peli.metar.nakyvyys);
	std::string lampotila 	= apuvalineet::tekstiksi(peli.metar.lampotila);
	std::string kastepiste 	= apuvalineet::tekstiksi(peli.metar.kastepiste);
	std::string pilvet;
	
	for (std::map<std::string, int>::iterator it=peli.metar.pilvet.begin(); it!=peli.metar.pilvet.end(); ++it) {
		pilvet += it->first + apuvalineet::tekstiksi(it->second) + " ";
	}

	tuuli = apuvalineet::muuta_pituus(tuuli, 3);
	voimakkuus = apuvalineet::muuta_pituus(voimakkuus, 2);

	kirjoita_tekstia(peli.kentta.nimi + " " + tuuli + voimakkuus + "KT " + nakyvyys + " " + lampotila + " / " + kastepiste + " " + pilvet + " " + paine, 50, 10);
}

void PeliView::piirra_odottavat() {
	int y = 120;
	
	if (peli.odottavat.size()) {
		kirjoita_tekstia("Odottavia koneita " + apuvalineet::tekstiksi(peli.odottavat.size()) + " kpl", asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), y - fontin_koko - 5);
	}
}

void PeliView::piirra_valinta() {
	piirra_kuva(image_cache::common().get("kuvat/tausta_valikko.png"), 0, 0);
	
	kirjoita_tekstia(peli.ohje, 50, 30);
	kirjoita_tekstia(peli.syote, 50, 50);
	
	SDL_Flip(ruutu);
}
