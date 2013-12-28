#include "peli.hpp"

bool poistetaanko(const lentokone& kone);

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
	std::ofstream ulos("virhedata.txt", std::ios::app);

	// TODO: Koska t�ss� on n�ytett�vi� asioita (teksti�), n�m� voisi harkita siirrett�v�n PeliView-luokkaan.
	switch (virhe) {
		case VIRHE_KORKEUS_ALA:
            virheteksti = "Korkeuden alaraja on " + apuvalineet::tekstiksi(asetukset.anna_asetus("selvityskorkeus_ala")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian alhainen aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_KORKEUS_YLA:
            virheteksti = "Korkeuden yl�raja on " + apuvalineet::tekstiksi(asetukset.anna_asetus("selvityskorkeus_yla")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian suuri aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISKORKEUS:
			virheteksti = "L�hestymisen yl�raja on " + apuvalineet::tekstiksi(asetukset.anna_asetus("maks_lahestymiskorkeus")) + " jalkaa";
			ulos << "Koneella liian suuri korkeus l�hestymiseen aikaan " << ohjelma.sekunnit() << " " << asetukset.anna_asetus("maks_lahestymiskorkeus") << std::endl;
			break;
		case VIRHE_LAHESTYMISNOPEUS:
			virheteksti = "L�hestymisen maksiminopeus on " + apuvalineet::tekstiksi(asetukset.anna_asetus("maks_lahestymisnopeus")) + " solmua";
			ulos << "Koneella liian suuri nopeus l�hestymiseen aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISSUUNTA:
			virheteksti ="Koneen suunnan tulee olla kiitotien suunnasta +- " + apuvalineet::tekstiksi(asetukset.anna_asetus("lahestymiskulma"));
			ulos << "Koneen suunnan tulee olla +- " << asetukset.anna_asetus("lahestymiskulma") << std::endl;
		case VIRHE_NOPEUS_ALA:
			virheteksti = "Nopeuden alaraja on " + apuvalineet::tekstiksi(asetukset.anna_asetus("selvitysnopeus_ala")) + " solmua";
			ulos << "Annettu selvitysnopeus on liian pieni aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_NOPEUS_YLA:
			virheteksti = "Nopeuden yl�raja on " + apuvalineet::tekstiksi(asetukset.anna_asetus("selvitysnopeus_yla")) + " solmua";
			ulos << "Annettu selvitysnopeus on suuri pieni aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_PORRASTUS:
			virheteksti = "Porrastuksen tulee olla pystysuuntaan " + apuvalineet::tekstiksi(asetukset.anna_asetus("porrastus_pysty")) + " jalkaa ja vaakasuuntaan " + apuvalineet::tekstiksi(asetukset.anna_asetus("porrastus_vaaka")) + " mailia";
			ulos << "Porrastusminimi alittui aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_OIKOTIE:
			virheteksti = "Oikotien saa antaa vasta " + apuvalineet::tekstiksi(asetukset.anna_asetus("oikotie")) + " jalassa";
			ulos << "Annettu oikotie liian aikaisin aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_ALUEELTA:
			virheteksti = "Kone poistui alueeltasi";
			ulos << "Kone poistui alueeltasi aikaan " << ohjelma.sekunnit() << std::endl;
			break;
	}

	ulos.close();
}

void Peli::luo_kone(IOhjelma& ohjelma) {
	int j = apuvalineet::arvo_luku(10, 100) % 2;
	apuvalineet::piste paikka;
	bool odotus;
	std::string tunnus = generoi_tunnus();

	if (j == LAHTEVA) {
		double suunta = kentta.kiitotiet[atis.lahtokiitotie].suunta;

		if (!onko_vapaata()) {
			paikka = kentta.kiitotiet[atis.lahtokiitotie].odotuspiste;
			odotus = true;
		} else {
			paikka = kentta.kiitotiet[atis.lahtokiitotie].alkupiste;
			odotus = false;
		}

		koneet.push_back(lentokone(*this, ohjelma, asetukset, tunnus, paikka, kentta.korkeus, 0.0, suunta, LAHTEVA, odotus));
		koneet.back().ulosmenopiste = navipisteet[apuvalineet::arvo_luku(0, navipisteet.size())];
		koneet.back().polttoaine = apuvalineet::arvo_luku(8000, 25000);

		if (koneet.back().odotus) {
			odottavat.push(koneet.back());
		}

	} else {
		int i = -1;
		do {
			i = apuvalineet::arvo_luku(0, navipisteet.size());
		} while (!onko_vapaata(SAAPUVA, i));

        koneet.push_back(lentokone(*this, ohjelma, asetukset, tunnus, navipisteet[i].paikka, navipisteet[i].lentokorkeus, navipisteet[i].lentonopeus, navipisteet[i].lentosuunta, SAAPUVA, false));
		koneet.back().polttoaine = apuvalineet::arvo_luku(3300, 5200);
	}

	tilasto tmp;

	tmp.tunnus = tunnus;
	tmp.sisaan = ohjelma.sekunnit();
	tmp.pois = 0.0;
	tmp.selvitykset = 0;

	ajat.push_back(tmp);
}

void Peli::lataa_kentta(std::string kenttaNimi) {
	std::clog << "Peli::lataa_kentta(" << kenttaNimi << ")" << std::endl;
	std::string kansio = "kentat/";
	std::string tmp;
    tmp = kansio + kenttaNimi;

	std::clog << tmp << std::endl;

	std::ifstream sisaan(tmp.c_str(), std::ios::in);
	std::vector <std::string> asiat;

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + tmp + " ei ole tai se ei aukea");
	}

	std::string nimi, tunnus;
	std::string rivi;

	while (std::getline(sisaan, rivi)) {
		asiat = apuvalineet::pilko_rivi(rivi, " ");

		if (asiat[0] == "N") {
			kentta.nimi = asiat[1];
		} else if (asiat[0] == "H") {
			kentta.korkeus = apuvalineet::luvuksi<double>(asiat[1]);
		} else if (asiat[0] == "P") {
			kentta.paikka.x = apuvalineet::luvuksi<double>(asiat[1]);
			kentta.paikka.y = apuvalineet::luvuksi<double>(asiat[2]);
		} else if (asiat[0] == "K") {
			apuvalineet::piste alku = apuvalineet::uusi_paikka(kentta.paikka, apuvalineet::luvuksi<double>(asiat[2]), apuvalineet::luvuksi<double>(asiat[3]));
			kiitotie tmpa(asiat[1], alku, apuvalineet::luvuksi<double>(asiat[5]), apuvalineet::luvuksi<double>(asiat[4]), apuvalineet::luvuksi<double>(asiat[6]), apuvalineet::luvuksi<double>(asiat[7]), asetukset.anna_asetus("lahestymispiste"), asetukset.anna_asetus("hidastuspiste"));

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
			throw std::runtime_error("Tiedosto " + kenttaNimi + " on v��r�ss� formaatissa");
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

void Peli::valitse_kone(const apuvalineet::piste& hiiri) {
	if (ohjelma.lue_hiiri()) {
		for (unsigned int i = 0; i < koneet.size(); ++i) {
			koneet[i].valittu = false;

			if (ohjelma.onko_alueella(hiiri, koneet[i].paikka)) {
				koneet[i].valittu = true;
			}
		}
	}
}

void Peli::tarkista_porrastus() {
	bool virheita = false;
	std::vector <int> alittuu;

	for (unsigned int i = 0; i < koneet.size(); ++i) {
		for (unsigned int j = 0; j < koneet.size(); ++j) {
			if (i ==j) {
				continue;
			}

			if (ohjelma.onko_alueella(koneet[i].paikka, koneet[j].paikka, 1.5) && std::abs(koneet[i].korkeus - koneet[j].korkeus) < 1000 && (koneet[i].korkeus > 1000 && koneet[j].korkeus > 1000)) {
				alittuu.push_back(i);
				alittuu.push_back(j);

				if (koneet[i].onko_porrastus == true || koneet[j].onko_porrastus == true) {
					virheita = true;
				}
			}
		}
	}

	if (virheita) {
		++porrastusvirheet;
		aseta_virhe(VIRHE_PORRASTUS);
	}

	for (unsigned int i = 0; i < koneet.size(); ++i) {
		koneet[i].onko_porrastus = true;
	}

	for (unsigned int i = 0; i < alittuu.size(); ++i) {
		koneet[alittuu[i]].onko_porrastus = false;
	}
}

void Peli::hoida_koneet() {
	std::vector <lentokone> :: iterator it;

    for (it = koneet.begin() ; it != koneet.end(); ++it) {
		it->poistetaan = false;
        if (it->tyyppi == Peli::LAHTEVA) {
            if (ohjelma.onko_alueella(it->paikka, it->ulosmenopiste.paikka)) {
				it->poistetaan = true;
                ++kasitellyt;
            }
        } else if (it->tyyppi == Peli::SAAPUVA) {
            if (it->anna_nopeus() < 4.0) {
				it->poistetaan = true;
                ++kasitellyt;
			}
        }

        if (it->paikka.x < 0 || it->paikka.x > asetukset.anna_asetus("ruutu_leveys") || it->paikka.y < 0 || it->paikka.y > asetukset.anna_asetus("ruutu_korkeus")) {
            aseta_virhe(VIRHE_ALUEELTA);
            it->poistetaan = true;
        }

		if (it->odotus) {
			if (onko_vapaata()) {
				it->paikka = kentta.kiitotiet[atis.lahtokiitotie].alkupiste;
				it->odotus = false;
				odottavat.pop();
			}
		}

		if (it->odotuskuvio > -1 && it->odotuskuvio < ohjelma.sekunnit()) {
			it->odotuskuvio += 120;
			it->ota_selvitys(it->anna_suunta() + 180, OIKEA);
		}

		if (it->tyyppi == Peli::LAHTEVA && it->anna_korkeus() < 1200 && it->odotus == false) {
			if (it->nopeus == 0) {
				it->muuta_selvitysnopeutta(asetukset.anna_asetus("alkunopeus"));
			}

			if (it->nopeus > 150) {
				it->ota_selvitys(kentta.kiitotiet[atis.lahtokiitotie].alkunousukorkeus, it->KORKEUS);
				it-ota_selvitys(asetukset.anna_asetus("alkunousunopeus"), it->NOPEUS);
				it->ota_selvitys(kentta.kiitotiet[atis.lahtokiitotie].alkunoususuunta, it->SUUNTA);
			}
		}

		it->liiku(ajan_muutos);
	}

	std::vector <lentokone> lyhyt;

	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (koneet[i].poistetaan == false) {
			lyhyt.push_back(koneet[i]);
		} else {
			std::clog << "Kone " << koneet[i].kutsutunnus << " poistuu..." << std::endl;
			std::vector <tilasto>::iterator ulos = std::find(ajat.begin(), ajat.end(), koneet[i].kutsutunnus);
			ulos->pois = ohjelma.sekunnit();
		}
	}

	koneet.clear();

	for (unsigned int i = 0; i < lyhyt.size(); ++i) {
		koneet.push_back(lyhyt[i]);
	}

	lyhyt.clear();
}

void Peli::lisaa_selvityksia() {
	int valittu = etsi_valittu_kone();
	std::vector <tilasto>::iterator lisattava = std::find(ajat.begin(), ajat.end(), koneet[valittu].kutsutunnus);
	++lisattava->selvitykset;
}

int Peli::etsi_valittu_kone() {
	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (koneet[i].valittu) {
			return i;
		}
	}

	return -1;
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
	metar.tuuli 		= apuvalineet::pyorista(apuvalineet::arvo_luku(0, 360), 5);
	metar.voimakkuus 	= apuvalineet::arvo_luku(asetukset.anna_asetus("tuuli_voimakkuus_ala"), asetukset.anna_asetus("tuuli_voimakkuus_yla"));
	metar.paine 		= apuvalineet::arvo_luku(asetukset.anna_asetus("ilmanpaine_ala"), asetukset.anna_asetus("ilmanpaine_yla"));
	metar.nakyvyys		= apuvalineet::arvo_luku(asetukset.anna_asetus("nakyvyys_ala"), asetukset.anna_asetus("nakyvyys_yla"));
	metar.lampotila		= apuvalineet::arvo_luku(asetukset.anna_asetus("lampotila_ala"), asetukset.anna_asetus("lampotila_yla"));
	metar.ilmankosteus	= apuvalineet::arvo_luku(asetukset.anna_asetus("ilmankosteus_ala"), asetukset.anna_asetus("ilmankosteus_yla"));
	metar.kastepiste	= metar.lampotila - ((100 - metar.ilmankosteus) / 5);

	int pilvia = apuvalineet::arvo_luku(asetukset.anna_asetus("pilvet_ala"), asetukset.anna_asetus("pilvet_yla"));
	std::vector <std::string> tyypit = lataa_pilvet("data/pilvet.txt");
	std::string tyyppi;

	for (int i = 0; i < pilvia; ++i) {
		tyyppi = tyypit[apuvalineet::arvo_luku(0, tyypit.size()-1)];
		metar.pilvet[tyyppi] = ((apuvalineet::arvo_luku(asetukset.anna_asetus("pilvenkorkeus_ala"), asetukset.anna_asetus("pilvenkorkeus_yla")) * 11 + 500) / 1000) * 100;
	}
}

bool Peli::onko_vapaata(int tyyppi, int piste) {
	if (tyyppi == LAHTEVA) {
		for (unsigned int i = 0; i < koneet.size(); ++i) {
			if (koneet[i].korkeus < (kentta.korkeus + asetukset.anna_asetus("porrastus_pysty")) && koneet[i].odotus == false) {
				return false;
			}
		}
	} else {
		for (unsigned int i = 0; i < koneet.size(); ++i) {
			if (apuvalineet::etaisyys(koneet[i].paikka, navipisteet[piste].paikka) < asetukset.anna_asetus("porrastus_vaaka")) {
				return false;
			}
		}

		return true;
	}

	return true;
}

bool Peli::tarkista_atis() {
	atis.lue_paineet("data/painerajat.txt");

	double vasta_lahto = std::cos(std::abs(kentta.kiitotiet[atis.lahtokiitotie].suunta - metar.tuuli));
	double vasta_lasku = std::cos(std::abs(kentta.kiitotiet[atis.laskukiitotie].suunta - metar.tuuli));
	double siirtopinta = atis.etsi_siirtopinta(metar.paine);

	if (vasta_lahto < 0 || vasta_lasku < 0 || siirtopinta != atis.siirtopinta) {
		return false;
	}

	std::clog << "tarkista_atis true" << std::endl;
	return true;
}

void Peli::Atis::lue_paineet(std::string nimi) {
	std::clog << "peli::atis.lue_paineet(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + nimi + " ei l�ydy tai se ei aukea");
	}

	double ala, yla, pinta;

	while (sisaan >> ala >> yla >> pinta) {
		Peli::Atis::Paine tmp;
		tmp.alaraja = ala;
		tmp.ylaraja = yla;
		tmp.siirtopinta = pinta;
		paineet.push_back(tmp);
	}

	sisaan.close();
}

double Peli::Atis::etsi_siirtopinta(double paine) {
	for (unsigned int i = 0; i < paineet.size(); ++i) {

		if (paine >= paineet[i].alaraja && paine < paineet[i].ylaraja) {
			return paineet[i].siirtopinta;
		}
	}

	return 0.0;
}
