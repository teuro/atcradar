#include "asetukset.h"
#include "peliview.hpp"
#include "ohjelma.hpp"
#include <iostream>
#include <stdexcept>

// Piirt‰‰ pelin.
void PeliView::piirra(IPiirtoPinta& piirtopinta) {
	piirtopinta.piirra_kuva("kuvat/tausta_peli.png", 0, 0);
	piirra_tilanne(piirtopinta);
	piirra_koneet(piirtopinta);
	piirra_navipisteet(piirtopinta);
	piirra_lentokentta(piirtopinta);
	piirra_ohje(piirtopinta, peli.ohje);
	piirra_odottavat(piirtopinta);
	piirtopinta.kirjoita_tekstia(peli.syote, 50, 70);
	piirra_metar(piirtopinta);
	piirtopinta.flip();
}

void PeliView::piirra_koneet(IPiirtoPinta& piirtopinta) {
	unsigned int vari;

	double lentokorkeus;
	double selvityskorkeus;

	unsigned int ok = 0x000000FF;
	unsigned int ng = 0xFF0000FF;

	int listauskorkeus = 90;

	for (unsigned int i = 0; i < peli.koneet.size(); ++i) {
		if (peli.koneet[i].anna_odotus() == false) {
			apuvalineet::piste loppupiste = apuvalineet::uusi_paikka(peli.koneet[i].paikka, peli.koneet[i].anna_suunta(), peli.koneet[i].anna_nopeus() * (60.0 / 3600.0));

			if (peli.koneet[i].onko_porrastus) {
				vari = ok;
			}
			else {
				vari = ng;
			}

			piirtopinta.rectangleColor(peli.koneet[i].paikka.x - 3, peli.koneet[i].paikka.y - 3, peli.koneet[i].paikka.x + 3, peli.koneet[i].paikka.y + 3, vari);
			piirtopinta.lineColor(peli.koneet[i].paikka.x, peli.koneet[i].paikka.y, loppupiste.x, loppupiste.y, vari);
			piirtopinta.circleColor(peli.koneet[i].paikka.x, peli.koneet[i].paikka.y, apuvalineet::nm2px(1.5), vari);

			piirtopinta.kirjoita_tekstia(peli.koneet[i].anna_kutsutunnus(), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y);

			piirtopinta.kirjoita_tekstia(peli.koneet[i].anna_kutsutunnus(), 30, listauskorkeus);

			if (peli.koneet[i].anna_reitin_koko() == 0) {
				if (peli.koneet[i].lahestymisselvitys) {
					piirtopinta.kirjoita_tekstia("ILS", 100, listauskorkeus);
				} else if (peli.koneet[i].anna_oikotie()) {
					piirtopinta.kirjoita_tekstia(peli.koneet[i].anna_ulosmenopiste().nimi, 100, listauskorkeus);
				}

				if (!peli.koneet[i].anna_lahestymisselvitys() && !peli.koneet[i].anna_oikotie()) {
					piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.koneet[i].anna_selvityssuunta()), 100, listauskorkeus);
				}
			} else {
				piirtopinta.kirjoita_tekstia(peli.koneet[i].anna_kohde().nimi, 100, listauskorkeus);
			}

			piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.koneet[i].anna_selvityskorkeus()), 160, listauskorkeus);
			piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.koneet[i].anna_selvitysnopeus()), 210, listauskorkeus);
			listauskorkeus += 20;


			if (peli.koneet[i].anna_tyyppi() == Peli::SAAPUVA) {
				if ((peli.koneet[i].anna_korkeus() / 100) < peli.atis.siirtopinta) {
					lentokorkeus = std::floor(peli.koneet[i].anna_korkeus());
				}
				else {
					lentokorkeus = std::floor(peli.koneet[i].anna_korkeus() / 100);
				}

				if ((peli.koneet[i].anna_selvityskorkeus() / 100) < peli.atis.siirtopinta) {
					selvityskorkeus = std::floor(peli.koneet[i].anna_selvityskorkeus());
				}
				else {
					selvityskorkeus = std::floor(peli.koneet[i].anna_selvityskorkeus() / 100);
				}

			}
			else {
				if (peli.koneet[i].anna_korkeus() < peli.atis.siirtokorkeus) {
					lentokorkeus = std::floor(peli.koneet[i].anna_korkeus());
				}
				else {
					lentokorkeus = std::floor(peli.koneet[i].anna_korkeus() / 100);
				}

				if (peli.koneet[i].anna_selvityskorkeus() < peli.atis.siirtokorkeus) {
					selvityskorkeus = std::floor(peli.koneet[i].anna_selvityskorkeus());
				}
				else {
					selvityskorkeus = std::floor(peli.koneet[i].anna_selvityskorkeus() / 100);
				}
			}

			piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(lentokorkeus) + " / " + apuvalineet::tekstiksi(selvityskorkeus), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + piirtopinta.get_fontinkoko() + 3);

			if (peli.koneet[i].anna_valittu()) {
				piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(peli.koneet[i].anna_nopeus())) + " / " + apuvalineet::tekstiksi(peli.koneet[i].anna_selvitysnopeus()), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + (2 * piirtopinta.get_fontinkoko()) + 3);
				piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(peli.koneet[i].anna_suunta())) + " / " + apuvalineet::tekstiksi(peli.koneet[i].anna_selvityssuunta()), peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + (3 * piirtopinta.get_fontinkoko()) + 3);

				if (peli.koneet[i].tyyppi == Peli::LAHTEVA) {
					piirtopinta.kirjoita_tekstia(peli.koneet[i].anna_ulosmenopiste().nimi, peli.koneet[i].paikka.x, peli.koneet[i].paikka.y + (4 * piirtopinta.get_fontinkoko()) + 3);
				}

				switch (peli.toiminto) {
				case apuvalineet::SUUNTA:
					piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_OHJE_SUUNTA), 50, 50);
					break;
				case apuvalineet::NOPEUS:
					piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_OHJE_NOPEUS), 50, 50);
					break;
				case apuvalineet::KORKEUS:
					piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_OHJE_KORKEUS), 50, 50);
					break;
				}

				piirtopinta.lineColor(peli.koneet[i].paikka.x, peli.koneet[i].paikka.y, peli.hiiren_paikka.x, peli.hiiren_paikka.y, vari);
				apuvalineet::vektori vek = apuvalineet::suunta_vektori(peli.koneet[i].paikka, peli.hiiren_paikka);

				int x = std::abs(peli.koneet[i].paikka.x + peli.hiiren_paikka.x) / 2;
				int y = std::abs(peli.koneet[i].paikka.y + peli.hiiren_paikka.y) / 2;

				piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(vek.pituus)) + " / " + apuvalineet::tekstiksi(std::floor(vek.suunta)), x, y);
			}
		}
	}
}

void PeliView::piirra_navipisteet(IPiirtoPinta& piirtopinta) {
	for (unsigned int i = 0; i < peli.navipisteet.size(); ++i) {
		apuvalineet::piste tmp = peli.navipisteet[i].paikka;
		piirtopinta.kirjoita_tekstia(peli.navipisteet[i].nimi, tmp.x, tmp.y);
		piirtopinta.trigonColor(tmp.x - 3, tmp.y + 3, tmp.x + 3, tmp.y + 3, tmp.x, tmp.y - 3, 456);
	}

	for (unsigned int i = 0; i < peli.sisapisteet.size(); ++i) {
		apuvalineet::piste tmp = peli.sisapisteet[i].paikka;
		piirtopinta.kirjoita_tekstia(peli.sisapisteet[i].nimi, tmp.x, tmp.y);
		piirtopinta.trigonColor(tmp.x - 3, tmp.y + 3, tmp.x + 3, tmp.y + 3, tmp.x, tmp.y - 3, 456);
	}
}

void PeliView::piirra_lentokentta(IPiirtoPinta& piirtopinta) {
	for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
		piirtopinta.lineColor(peli.kentta.kiitotiet[i].alkupiste.x, peli.kentta.kiitotiet[i].alkupiste.y, peli.kentta.kiitotiet[i].loppupiste.x, peli.kentta.kiitotiet[i].loppupiste.y, 0x223344FF);
		piirtopinta.kirjoita_tekstia(peli.kentta.kiitotiet[i].nimi, peli.kentta.kiitotiet[i].alkupiste.x, peli.kentta.kiitotiet[i].alkupiste.y);

		piirtopinta.circleColor(peli.kentta.kiitotiet[i].lahestymispiste.x, peli.kentta.kiitotiet[i].lahestymispiste.y, apuvalineet::nm2px(0.5), 0xAAAAAAFF);
	}
}

void PeliView::piirra_tilanne(IPiirtoPinta& piirtopinta) {
	std::string teksti = "K‰sitellyt " + apuvalineet::tekstiksi(peli.kasitellyt) + std::string("/") + apuvalineet::tekstiksi(asetukset.anna_asetus("vaadittavat_kasitellyt"));
	piirtopinta.kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), 20);

	teksti = "porrastusvirheet " + apuvalineet::tekstiksi(peli.porrastusvirheet) + std::string("/") + apuvalineet::tekstiksi(asetukset.anna_asetus("maks_porrastusvirhe"));
	piirtopinta.kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), 40);

	teksti = "muut virheet " + apuvalineet::tekstiksi(peli.muut_virheet);
	piirtopinta.kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), 60);
}

void PeliView::piirra_ohje(IPiirtoPinta& piirtopinta, std::string ohje) {
	piirtopinta.kirjoita_tekstia(ohje.c_str(), 50, 30);
}

void PeliView::piirra_metar(IPiirtoPinta& piirtopinta) {
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

	piirtopinta.kirjoita_tekstia(peli.kentta.nimi + " " + tuuli + voimakkuus + "KT " + nakyvyys + " " + lampotila + " / " + kastepiste + " " + pilvet + " " + paine, 50, 10);
}

void PeliView::piirra_odottavat(IPiirtoPinta& piirtopinta) {
	int y = 120;

	if (peli.odottavat.size()) {
		piirtopinta.kirjoita_tekstia("Odottavia koneita " + apuvalineet::tekstiksi(peli.odottavat.size()) + " kpl", asetukset.anna_asetus("ruutu_leveys") - asetukset.anna_asetus("info_leveys"), y - piirtopinta.get_fontinkoko() - 5);
	}
}
