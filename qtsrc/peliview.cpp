#include "PeliView.hpp"

// Piirtää pelin.
void PeliView::piirra(IPiirtoPinta& piirtopinta) {
    piirra_tilanne(piirtopinta);
	piirra_koneet(piirtopinta);
	piirra_navipisteet(piirtopinta);
	piirra_lentokentta(piirtopinta);
    piirra_ohje(piirtopinta, peli.ohje);
	piirra_odottavat(piirtopinta);

    if (peli.koska_uusi_kone > 0) {
        piirtopinta.kirjoita_tekstia(QObject::tr("Uusi kone tulee").toStdString() + " " + apuvalineet::muotoile_aika("%m:%s", peli.koska_uusi_kone - peli.anna_pelin_kello()), asetukset.anna_asetus("ruutu_leveys")-150, 20);
    } else {
        piirtopinta.kirjoita_tekstia(QObject::tr("Uutta konetta ei tule").toStdString(), asetukset.anna_asetus("ruutu_leveys")-150, 20);
    }

    if (peli.odottavat.size()) {
        piirtopinta.kirjoita_tekstia(QObject::tr("Seuraava kone:").toStdString() + apuvalineet::muotoile_aika("%m:%s", peli.edellinen_kone_lahto + asetukset.anna_asetus("lahtevien_porrastus") * 60 - peli.anna_pelin_kello()), asetukset.anna_asetus("ruutu_leveys")-150, 80);
    }

    piirtopinta.kirjoita_tekstia(QObject::tr("pelin kello").toStdString() + " " + apuvalineet::muotoile_aika("%h:%m:%s", peli.anna_pelin_kello()), asetukset.anna_asetus("ruutu_leveys")-150, 40);

    piirra_metar(piirtopinta);
    piirra_jaljet(piirtopinta);
	piirtopinta.flip();
}

void PeliView::piirra_koneet(IPiirtoPinta& piirtopinta) {
    unsigned int vari;

	double lentokorkeus;
	double selvityskorkeus;

    unsigned int ok = IPiirtoPinta::NORMAALI;
    unsigned int ng = IPiirtoPinta::VIRHE;
    unsigned int valittu = IPiirtoPinta::VALITTU;

    int listauskorkeus = 160;

    piirtopinta.kirjoita_tekstia(QObject::tr("Koneita").toStdString() + " " + apuvalineet::tekstiksi(peli.koneet.size()) + " / " + apuvalineet::tekstiksi(asetukset.anna_asetus("maks_konemaara")), 30, listauskorkeus - 20);

    int tyyppi = peli.koneet.front()->anna_tyyppi();
    piirtopinta.kirjoita_tekstia(QObject::tr("Saapuvat koneet:").toStdString() + " ", 30, listauskorkeus);
    for (std::list <lentokone*> :: iterator it = peli.koneet.begin(); it != peli.koneet.end(); ++it) {
        if ((*it)->anna_odotus() == false) {
            apuvalineet::piste loppupiste = apuvalineet::uusi_paikka((*it)->paikka, (*it)->anna_suunta(), (*it)->anna_nopeus() * (60.0 / 3600.0));

            if (tyyppi == (*it)->anna_tyyppi()) {
                listauskorkeus += 20;
            } else {
                piirtopinta.kirjoita_tekstia(QObject::tr("Nousevat koneet:").toStdString() + " ", 30, listauskorkeus+20);
                listauskorkeus += 40;
                tyyppi = (*it)->anna_tyyppi();
            }

            if ((*it)->onko_porrastus) {
				vari = ok;
                if (peli.valittuKone == (*it)) {
                    vari = valittu;
                }
            } else {
                vari = ng;
			}

            piirtopinta.rectangleColor((*it)->paikka.x - 3, (*it)->paikka.y - 3, (*it)->paikka.x + 3, (*it)->paikka.y + 3, vari);
            piirtopinta.lineColor((*it)->paikka.x, (*it)->paikka.y, loppupiste.x, loppupiste.y, vari);
            piirtopinta.circleColor((*it)->paikka.x, (*it)->paikka.y, apuvalineet::nm2px(1.5), vari);

            piirtopinta.kirjoita_tekstia((*it)->anna_kutsutunnus(), (*it)->paikka.x, (*it)->paikka.y, vari);

            piirtopinta.kirjoita_tekstia((*it)->anna_kutsutunnus(), 30, listauskorkeus, vari);

            if ((*it)->anna_reitin_koko() == 0) {
                if ((*it)->lahestymisselvitys && !(*it)->anna_laskuselvitys()) {
					piirtopinta.kirjoita_tekstia("ILS", 100, listauskorkeus);
                } else if ((*it)->anna_oikotie()) {
                    piirtopinta.kirjoita_tekstia((*it)->anna_ulosmenopiste().nimi, 100, listauskorkeus, vari);
                } else if ((*it)->anna_laskuselvitys()) {
                    piirtopinta.kirjoita_tekstia("LAND", 100, listauskorkeus);
                }

                if (!(*it)->anna_lahestymisselvitys() && !(*it)->anna_oikotie()) {
                    piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi((*it)->anna_selvityssuunta()), 100, listauskorkeus, vari);
				}
			} else {
                piirtopinta.kirjoita_tekstia((*it)->anna_kohde().nimi, 100, listauskorkeus, vari);
			}

            piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi((*it)->anna_selvityskorkeus()), 160, listauskorkeus, vari);
            piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi((*it)->anna_selvitysnopeus()), 210, listauskorkeus, vari);

            if ((*it)->anna_tyyppi() == Peli::SAAPUVA) {
                if (((*it)->anna_korkeus() / 100) < atis.anna_siirtopinta()) {
                    lentokorkeus = std::floor((*it)->anna_korkeus());
				}
				else {
                    lentokorkeus = std::floor((*it)->anna_korkeus() / 100);
				}

                if (((*it)->anna_selvityskorkeus() / 100) < atis.anna_siirtopinta()) {
                    selvityskorkeus = std::floor((*it)->anna_selvityskorkeus());
				}
				else {
                    selvityskorkeus = std::floor((*it)->anna_selvityskorkeus() / 100);
				}

			}
			else {
                if ((*it)->anna_korkeus() < atis.anna_siirtokorkeus()) {
                    lentokorkeus = std::floor((*it)->anna_korkeus());
				}
				else {
                    lentokorkeus = std::floor((*it)->anna_korkeus() / 100);
				}

                if ((*it)->anna_selvityskorkeus() < atis.anna_siirtokorkeus()) {
                    selvityskorkeus = std::floor((*it)->anna_selvityskorkeus());
				}
				else {
                    selvityskorkeus = std::floor((*it)->anna_selvityskorkeus() / 100);
				}
			}

            piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(lentokorkeus) + " / " + apuvalineet::tekstiksi(selvityskorkeus), (*it)->paikka.x, (*it)->paikka.y + piirtopinta.get_fontinkoko() + 3, vari);

            if (peli.valittuKone == (*it)) {
                piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(std::floor((*it)->anna_nopeus())) + " / " + apuvalineet::tekstiksi((*it)->anna_selvitysnopeus()), (*it)->paikka.x, (*it)->paikka.y + (2 * piirtopinta.get_fontinkoko()) + 3, vari);
                piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(std::floor((*it)->anna_suunta())) + " / " + apuvalineet::tekstiksi((*it)->anna_selvityssuunta()), (*it)->paikka.x, (*it)->paikka.y + (3 * piirtopinta.get_fontinkoko()) + 3, vari);

                if ((*it)->tyyppi == Peli::LAHTEVA) {
                    piirtopinta.kirjoita_tekstia((*it)->anna_ulosmenopiste().nimi, (*it)->paikka.x, (*it)->paikka.y + (4 * piirtopinta.get_fontinkoko()) + 3, vari);
				}

                piirtopinta.lineColor((*it)->paikka.x, (*it)->paikka.y, peli.hiiren_paikka.x, peli.hiiren_paikka.y, vari);
                apuvalineet::vektori vek = apuvalineet::suunta_vektori((*it)->paikka, peli.hiiren_paikka);

                int x = std::abs((*it)->paikka.x + peli.hiiren_paikka.x) / 2;
                int y = std::abs((*it)->paikka.y + peli.hiiren_paikka.y) / 2;

                piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(std::floor(vek.pituus)) + " / " + apuvalineet::tekstiksi(std::floor(vek.suunta)), x, y, vari);
			}
		}
	}
}

void PeliView::piirra_navipisteet(IPiirtoPinta& piirtopinta) {
    for (unsigned int i = 0; i < peli.navipisteet.size(); ++i) {
        apuvalineet::piste tmp = peli.navipisteet[i].paikka;
        piirtopinta.kirjoita_tekstia(peli.navipisteet[i].nimi, tmp.x, tmp.y);
        piirtopinta.trigonColor(tmp.x - 3, tmp.y + 3, tmp.x + 3, tmp.y + 3, tmp.x, tmp.y - 3, IPiirtoPinta::NORMAALI);
	}

    for (unsigned int i = 0; i < peli.sisapisteet.size(); ++i) {
        apuvalineet::piste tmp = peli.sisapisteet[i].paikka;
        piirtopinta.kirjoita_tekstia(peli.sisapisteet[i].nimi, tmp.x, tmp.y);
        piirtopinta.trigonColor(tmp.x - 3, tmp.y + 3, tmp.x + 3, tmp.y + 3, tmp.x, tmp.y - 3, IPiirtoPinta::NORMAALI);
	}
}

void PeliView::piirra_lentokentta(IPiirtoPinta& piirtopinta) {
    for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
        piirtopinta.lineColor(peli.kentta.kiitotiet[i].alkupiste.x, peli.kentta.kiitotiet[i].alkupiste.y, peli.kentta.kiitotiet[i].loppupiste.x, peli.kentta.kiitotiet[i].loppupiste.y, IPiirtoPinta::NORMAALI);
        piirtopinta.kirjoita_tekstia(peli.kentta.kiitotiet[i].nimi, peli.kentta.kiitotiet[i].alkupiste.x, peli.kentta.kiitotiet[i].alkupiste.y);

        piirtopinta.circleColor(peli.kentta.kiitotiet[i].lahestymispiste.x, peli.kentta.kiitotiet[i].lahestymispiste.y, apuvalineet::nm2px(0.5), IPiirtoPinta::NORMAALI);
	}
}

void PeliView::piirra_tilanne(IPiirtoPinta& piirtopinta) {
    std::string teksti = QObject::tr("Hoidetut").toStdString() + " " + apuvalineet::tekstiksi(peli.kasitellyt) + std::string("/") + apuvalineet::tekstiksi(asetukset.anna_asetus("vaadittavat_kasitellyt"));
    piirtopinta.kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys")-150, 100);

    teksti = QObject::tr("porrastusvirheet").toStdString() + " " + apuvalineet::tekstiksi(peli.porrastusvirheet) + std::string("/") + apuvalineet::tekstiksi(asetukset.anna_asetus("maks_porrastusvirhe"));
    piirtopinta.kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys")-150, 120);

    teksti = QObject::tr("muut virheet").toStdString() + " " + apuvalineet::tekstiksi(peli.muut_virheet);
    piirtopinta.kirjoita_tekstia(teksti, asetukset.anna_asetus("ruutu_leveys")-150, 140);
}

void PeliView::piirra_ohje(IPiirtoPinta& piirtopinta, std::string ohje) {
    piirtopinta.kirjoita_tekstia(ohje.c_str(), asetukset.anna_asetus("ruutu_leveys")-150, 100);
}

void PeliView::piirra_metar(IPiirtoPinta& piirtopinta) {
    std::string tuuli 		= apuvalineet::tekstiksi(peli.metar.anna_tuuli());
    std::string voimakkuus 	= apuvalineet::tekstiksi(peli.metar.anna_voimakkuus());
    std::string paine 		= apuvalineet::tekstiksi(peli.metar.anna_paine());
    std::string nakyvyys 	= apuvalineet::tekstiksi(peli.metar.anna_nakyvyys());
    std::string lampotila 	= apuvalineet::tekstiksi(peli.metar.anna_lampotila());
    std::string kastepiste 	= apuvalineet::tekstiksi(peli.metar.anna_kastepiste());
    std::string pilvet      = peli.metar.anna_pilvet();

	tuuli = apuvalineet::muuta_pituus(tuuli, 3);
	voimakkuus = apuvalineet::muuta_pituus(voimakkuus, 2);

    piirtopinta.kirjoita_tekstia(peli.kentta.nimi + " " + tuuli + voimakkuus + "KT " + nakyvyys + " " + lampotila + " / " + kastepiste + " " + pilvet + " " + paine, 150, 10);
}

void PeliView::piirra_odottavat(IPiirtoPinta& piirtopinta) {
    if (peli.odottavat.size()) {
        piirtopinta.kirjoita_tekstia(QObject::tr("Odottavia koneita").toStdString() + " " + apuvalineet::tekstiksi(peli.odottavat.size()) + " kpl", asetukset.anna_asetus("ruutu_leveys")-150, 60);
	}
}

void PeliView::piirra_jaljet(IPiirtoPinta &piirtopinta) {
    for (unsigned int i = 0; i < peli.jaljet.size(); ++i) {
        piirtopinta.circleColor(peli.jaljet[i].x, peli.jaljet[i].y, 1, IPiirtoPinta::NORMAALI);
    }
}
