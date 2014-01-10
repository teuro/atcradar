#include "peli.hpp"

bool poistetaanko(const lentokone& kone);

Peli::Peli(IAsetukset& a, Kieli& kieli, std::string kentta, Metar& m, Atis &at) : asetukset(a), koska_uusi_kone(1), metar(m), atis(at) {
	lataa_kentta(kentta);
	ohje = " ";
	porrastusvirheet = 0;
	muut_virheet = 0;
	kasitellyt = 0;
	koska_metar = asetukset.anna_asetus("koska_metar");
	lataa_tunnukset("data/tunnukset.txt");
	generoi_metar();
    valittuKone = NULL;
}

void Peli::lataa_tunnukset(std::string tunnukset) {
	std::clog << "peli::lataa_tunnukset(" << tunnukset << ")" << std::endl;
	std::string tmp;

	std::ifstream sisaan(tunnukset.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + tunnukset + " ei ole tai se ei aukea");
	}

	while (sisaan >> tmp) {
		this->tunnukset.push_back(tmp);
	}
}

void Peli::aseta_virhe(int virhe) {
	std::clog << "peli::aseta_virhe(" << virhe << ") " << std::endl;
	virheteksti = " ";
    ++muut_virheet;
}

void Peli::luo_kone(double aika) {
    std::clog << "Peli::luo_kone(" << aika << ")" << std::endl;
	int j = apuvalineet::arvo_luku(10, 100) % 2;
	apuvalineet::piste paikka;
	bool odotus;
	std::string tunnus = generoi_tunnus();

	if (j == LAHTEVA) {
        std::vector <kiitotie> :: iterator haku_lahto = std::find(kentta.kiitotiet.begin(), kentta.kiitotiet.end(), atis.anna_lahtokiitotie());
        double suunta = haku_lahto->alkunoususuunta;

		if (!onko_vapaata()) {
            paikka = haku_lahto->odotuspiste;
			odotus = true;
		} else {
            paikka = haku_lahto->alkupiste;
			odotus = false;
		}

        koneet.push_back(new lentokone(tunnus, paikka, kentta.korkeus, 0.0, suunta, LAHTEVA, odotus));
        koneet.back()->aseta_ulosmenopiste(navipisteet[apuvalineet::arvo_luku(0, navipisteet.size())]);

        if (koneet.back()->anna_odotus()) {
            odottavat.push(*koneet.back());
		}

	} else {
		int i = -1;
		do {
			i = apuvalineet::arvo_luku(0, navipisteet.size());
		} while (!onko_vapaata(SAAPUVA, i));

        koneet.push_back(new lentokone(tunnus, navipisteet[i].paikka, navipisteet[i].lentokorkeus, navipisteet[i].lentonopeus, navipisteet[i].lentosuunta, SAAPUVA, false));
	}

	tilasto tmp;

	tmp.tunnus = tunnus;
    tmp.sisaan = aika;
	tmp.pois = 0.0;
	tmp.selvitykset = 0;

	ajat.push_back(tmp);
}

void Peli::lataa_kentta(std::string kenttaNimi) {
	std::clog << "Peli::lataa_kentta(" << kenttaNimi << ")" << std::endl;
	std::string kansio = "kentat/";
	std::string tmp = kansio + kenttaNimi;

	std::clog << tmp << std::endl;

	std::ifstream sisaan(tmp.c_str(), std::ios::in);
	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + tmp + " ei ole tai se ei aukea");
	}

	std::string rivi;

	while (std::getline(sisaan, rivi)) {
		std::vector <std::string> asiat = apuvalineet::pilko_rivi(rivi, " ");

		if (asiat[0] == "N") {
			kentta.nimi = asiat[1];
            std::clog << "Kentta.nimi = " << kentta.nimi << std::endl;
		} else if (asiat[0] == "H") {
			kentta.korkeus = apuvalineet::luvuksi<double>(asiat[1]);
            std::clog << "Kentta.nimi = " << kentta.korkeus << std::endl;
		} else if (asiat[0] == "P") {
			kentta.paikka.x = apuvalineet::luvuksi<double>(asiat[1]);
			kentta.paikka.y = apuvalineet::luvuksi<double>(asiat[2]);

            std::clog << "Kentta.paikka = " << kentta.paikka.x << ", " << kentta.paikka.y << std::endl;
		} else if (asiat[0] == "K") {
			apuvalineet::piste alku = apuvalineet::uusi_paikka(kentta.paikka, apuvalineet::luvuksi<double>(asiat[2]), apuvalineet::luvuksi<double>(asiat[3]));
			kiitotie tmpa(asiat[1], alku, apuvalineet::luvuksi<double>(asiat[5]), apuvalineet::luvuksi<double>(asiat[4]), apuvalineet::luvuksi<double>(asiat[6]), apuvalineet::luvuksi<double>(asiat[7]), asetukset.anna_asetus("lahestymispiste"), asetukset.anna_asetus("hidastuspiste"));
			tmpa.liukupolku = 3.0;

			kentta.kiitotiet.push_back(tmpa);
		} else if (asiat[0] == "L") {
			apuvalineet::piste paikka;
			paikka = apuvalineet::uusi_paikka(kentta.paikka, apuvalineet::luvuksi<double>(asiat[2]), apuvalineet::luvuksi<double>(asiat[3]));

			navipiste tmp(apuvalineet::tekstiksi(asiat[1]), paikka);

			sisapisteet.push_back(tmp);
		} else if (asiat[0] == "U") {
			apuvalineet::piste paikka;

			paikka = apuvalineet::uusi_paikka(kentta.paikka, apuvalineet::luvuksi<double>(asiat[2]), apuvalineet::luvuksi<double>(asiat[3]));

			navipiste tmp(asiat[1], paikka, apuvalineet::luvuksi<double>(asiat[4]), apuvalineet::luvuksi<double>(asiat[5]), apuvalineet::luvuksi<double>(asiat[6]));

			navipisteet.push_back(tmp);
		} else {
			throw std::runtime_error("Tiedosto " + kenttaNimi + " on väärässä formaatissa");
		}

		asiat.clear();
	}

	sisaan.close();
}

std::string Peli::generoi_tunnus() {
	std::string tunniste = tunnukset[apuvalineet::arvo_luku(0, tunnukset.size())];
	std::string luku = apuvalineet::tekstiksi(apuvalineet::arvo_luku(0, 999));

	return tunniste+luku;
}

void Peli::aseta_hiiren_paikka(const apuvalineet::piste& hiiri) {
    hiiren_paikka = hiiri;
}

void Peli::valitse_kone(const apuvalineet::piste& hiiri) {
    valittuKone = NULL;

    for (std::list <lentokone*> :: iterator it = koneet.begin(); it != koneet.end(); ++it) {
        if (apuvalineet::onko_alueella(hiiri, (*it)->paikka)) {
            valittuKone = (*it);
            break;
		}
	}
}

void Peli::tarkista_porrastus() {
	bool virheita = false;
	std::vector <int> alittuu;

    for (std::list <lentokone*> :: iterator it = koneet.begin(); it != koneet.end(); ++it) {
        for (std::list <lentokone*> :: iterator jt = koneet.begin(); jt != koneet.end(); ++jt) {
            if (*it == *jt) {
				continue;
			}

            if (apuvalineet::onko_alueella((*it)->paikka, (*jt)->paikka, 1.5) && std::abs((*it)->anna_korkeus() - (*jt)->anna_korkeus()) < 1000 && ((*it)->anna_korkeus() > 1000 && (*jt)->anna_korkeus() > 1000)) {
                if ((*it)->onko_porrastus == true || (*jt)->onko_porrastus == true) {
					virheita = true;
				}
			}
		}
	}

	if (virheita) {
		++porrastusvirheet;
		aseta_virhe(VIRHE_PORRASTUS);
	}

    for (std::list <lentokone*> :: iterator it = koneet.begin(); it != koneet.end(); ++it) {
        (*it)->onko_porrastus = true;
	}
}

void Peli::hoida_koneet(double intervalliMs) {
    std::vector <kiitotie> :: iterator haku_lahto = std::find(kentta.kiitotiet.begin(), kentta.kiitotiet.end(), atis.anna_lahtokiitotie());

    for (std::list <lentokone*> :: iterator it = koneet.begin(); it != koneet.end(); ++it) {
        (*it)->aseta_poistetaan(false);
        if ((*it)->tyyppi == Peli::LAHTEVA) {
            if (apuvalineet::onko_alueella((*it)->paikka, (*it)->anna_ulosmenopiste().paikka)) {
                (*it)->aseta_poistetaan(true);
                ++kasitellyt;
            }
        } else if ((*it)->tyyppi == Peli::SAAPUVA) {
            if ((*it)->anna_nopeus() < 4.0) {
                (*it)->aseta_poistetaan(true);
                ++kasitellyt;
			}
        }

        if ((*it)->paikka.x < 0 || (*it)->paikka.x > asetukset.anna_asetus("ruutu_leveys") || (*it)->paikka.y < 0 || (*it)->paikka.y > asetukset.anna_asetus("ruutu_korkeus")) {
            aseta_virhe(VIRHE_ALUEELTA);
            (*it)->aseta_poistetaan(true);
        }

        if ((*it)->anna_odotus()) {
			if (onko_vapaata()) {
                (*it)->paikka = haku_lahto->alkupiste;
                (*it)->aseta_odotus(false);
				odottavat.pop();
			}
		}
/*
		if (it->anna_odotuskuvio() > -1 && it->anna_odotuskuvio() < ohjelma.sekunnit()) {
			it->aseta_odotuskuvio(120);
			it->ota_selvitys(it->anna_suunta() + 180, apuvalineet::OIKEA);
        }*/

        if ((*it)->tyyppi == Peli::LAHTEVA && (*it)->anna_korkeus() < 1200 && (*it)->anna_odotus() == false) {
            if ((*it)->anna_nopeus() == 0) {
                (*it)->ota_selvitys(asetukset.anna_asetus("alkunopeus"), apuvalineet::NOPEUS);
			}

            if ((*it)->anna_nopeus() > 150) {
                (*it)->ota_selvitys(haku_lahto->alkunousukorkeus, apuvalineet::KORKEUS);
                (*it)->ota_selvitys(asetukset.anna_asetus("alkunousunopeus"), apuvalineet::NOPEUS);
                (*it)->ota_selvitys(haku_lahto->alkunoususuunta, apuvalineet::SUUNTA);
			}
		}

        (*it)->liiku(intervalliMs);
	}
}

void Peli::lisaa_selvityksia() {
    std::vector <tilasto>::iterator lisattava = std::find(ajat.begin(), ajat.end(), valittuKone->anna_kutsutunnus());
	++lisattava->selvitykset;
}

std::vector <std::string> Peli::lataa_pilvet(std::string pilvet) {
	std::ifstream sisaan(pilvet.c_str(), std::ios::in);
	std::vector <std::string> tyypit;
	std::string pilvi;

	if (!sisaan) {
		throw std::runtime_error("Tiedosto " + pilvet + " ei aukea");
	}

	while (sisaan >> pilvi) {
		tyypit.push_back(pilvi);
	}

	sisaan.close();

	return tyypit;
}

void Peli::generoi_metar() {
    metar.aseta_tuuli(apuvalineet::pyorista(apuvalineet::arvo_luku(0, 360), 5));
    metar.aseta_voimakkuus(apuvalineet::arvo_luku(asetukset.anna_asetus("tuuli_voimakkuus_ala"), asetukset.anna_asetus("tuuli_voimakkuus_yla")));
    metar.aseta_paine(apuvalineet::arvo_luku(asetukset.anna_asetus("ilmanpaine_ala"), asetukset.anna_asetus("ilmanpaine_yla")));
    metar.aseta_nakyvyys(apuvalineet::arvo_luku(asetukset.anna_asetus("nakyvyys_ala"), asetukset.anna_asetus("nakyvyys_yla")));
    metar.aseta_lampotila(apuvalineet::arvo_luku(asetukset.anna_asetus("lampotila_ala"), asetukset.anna_asetus("lampotila_yla")));
    metar.aseta_ilmankosteus(apuvalineet::arvo_luku(asetukset.anna_asetus("ilmankosteus_ala"), asetukset.anna_asetus("ilmankosteus_yla")));
    metar.aseta_kastepiste(metar.anna_lampotila() - ((100 - metar.anna_ilmankosteus()) / 5));

	int pilvia = apuvalineet::arvo_luku(asetukset.anna_asetus("pilvet_ala"), asetukset.anna_asetus("pilvet_yla"));
	std::vector <std::string> tyypit = lataa_pilvet("data/pilvet.txt");
	std::string tyyppi;
    std::string pilvi;

	for (int i = 0; i < pilvia; ++i) {
		tyyppi = tyypit[apuvalineet::arvo_luku(0, tyypit.size()-1)];
        pilvi += apuvalineet::pyorista(apuvalineet::arvo_luku(asetukset.anna_asetus("pilvenkorkeus_ala"), asetukset.anna_asetus("pilvenkorkeus_yla")), 100);
	}

    metar.aseta_pilvet(pilvi);
}

bool Peli::onko_vapaata(int tyyppi, int piste) {
	if (tyyppi == LAHTEVA) {
        for (std::list <lentokone*> :: iterator it = koneet.begin(); it != koneet.end(); ++it) {
            if ((*it)->anna_korkeus() < (kentta.korkeus + asetukset.anna_asetus("porrastus_pysty")) && (*it)->anna_odotus() == false) {
				return false;
			}
		}
	} else {
        for (std::list <lentokone*> :: iterator it = koneet.begin(); it != koneet.end(); ++it) {
            if (apuvalineet::etaisyys((*it)->paikka, navipisteet[piste].paikka) < asetukset.anna_asetus("porrastus_vaaka")) {
				return false;
			}
		}

		return true;
	}

	return true;
}

bool Peli::tarkista_atis() {
    std::vector <kiitotie> :: iterator haku_lahto = std::find(kentta.kiitotiet.begin(), kentta.kiitotiet.end(), atis.anna_lahtokiitotie());

    atis.downloadPrressureLimit("data/painerajat.txt", atis.anna_siirtokorkeus());

    double vasta_lahto = std::cos(std::abs(haku_lahto->suunta - metar.anna_tuuli()));
    double vasta_lasku = std::cos(std::abs(haku_lahto->suunta - metar.anna_tuuli()));
    double siirtopinta = atis.calculateTL(metar.anna_paine());

    if (vasta_lahto < 0 || vasta_lasku < 0 || siirtopinta != atis.anna_siirtopinta()) {
		return false;
    }

	return true;
}
