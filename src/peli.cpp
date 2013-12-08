// peli.cpp
#include "peli.hpp"
#include "ohjelma.hpp"
#include "lukija.hpp"
#include "ajastin.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace peli {
	const float ajan_muutos = 0.02;
	std::vector <std::string> tunnukset;
	std::string syote;

	static void lataa_kentta(std::string kentta);
	static void lataa_tunnukset(std::string tunnukset);

	static void lisaa_selvityksia();

	std::vector <tilasto> ajat;

	std::string ohje = " ";
	std::string virheteksti = " ";

	int kasitellyt = 0;

	std::vector <lentokone> koneet;
	std::vector <lentokone> odottavat;

	std::vector <navipiste> navipisteet;
	std::vector <navipiste> sisapisteet;

	static std::string generoi_tunnus();

	static void valitse_kone(const apuvalineet::piste& hiiri);

	static void tarkista_porrastus();
	static void lataa_navipisteet(std::string nimi);
	static void lataa_lahipisteet(std::string nimi);

	static bool onko_vapaata();
	static void hoida_koneet();
	static void poista_kone(int kone);
	static int etsi_valittu_kone();

	static void pyyda_atis();
	static bool tarkista_atis();
	static void generoi_metar();

	lentokentta kentta;

	int alku;
	int toiminto;
	int koska_uusi_kone = 1;
	int koska_metar;
	int porrastusvirheet = 0;
	int muut_virheet = 0;

	namespace atis {
		int lahtokiitotie = -1;
		int laskukiitotie = -1;
		std::string lahto;
		std::string lasku;
		double siirtopinta = -1;
		int siirtokorkeus;
		bool ok = false;

		struct paine {
			double alaraja;
			double ylaraja;
			double siirtopinta;
		};

		std::vector <paine> paineet;
		void lue_paineet(std::string nimi);
		double etsi_siirtopinta(double paine);
	}

	namespace metar {
		int tuuli;
		int voimakkuus;
		int puuskat;
		double paine;
		int nakyvyys;
		int lampotila;
		int kastepiste;
		int ilmankosteus;
	}
}

// Pelin pääfunktio.
int peli::aja() {
	std::clog << "peli::aja()" << std::endl;

	//ohjelma::lataa_asetukset("asetukset.ini");
	lataa_tunnukset("data\\tunnukset.txt");

	lataa_kentta("EFRO");

	std::srand(std::time(NULL));

	generoi_metar();
	bool loppu = false, piirretty = false;

	pyyda_atis();

	// Nollataan kello.
	float pelin_kello = ohjelma::sekunnit(true);

	ohjelma::tyhjenna_syote();

	apuvalineet::piste hiiri;
	syotteenluku lukija;
	koska_metar = ohjelma::anna_asetus("koska_metar");

	std::vector <ajastin> ajastimet;

	ajastimet.push_back(ajastin("metar", ohjelma::anna_asetus("koska_metar"), 0));

	luo_kone();
	luo_kone();

	while (!loppu) {
		alku = ohjelma::sekunnit();
		hiiri = ohjelma::anna_hiiri();

		valitse_kone(hiiri);

		if (porrastusvirheet >= ohjelma::anna_asetus("maks_porrastusvirhe")) {
			std::clog << "Porrastusvirheet täynnä" << std::endl;
			loppu = true;
		}

		if (alku == koska_uusi_kone) {
			if (koneet.size() < ohjelma::anna_asetus("maks_konemaara")) {
				luo_kone();
				koska_uusi_kone += apuvalineet::arvo_luku(ohjelma::anna_asetus("koska_uusi_ala"), ohjelma::anna_asetus("koska_uusi_yla"));
			}

			koska_uusi_kone += 15;

			std::clog << "Seuraava kone luodaan " << (koska_uusi_kone - alku) << " sekunnin kuluttua" << std::endl;
		}

		if (alku == koska_metar) {
			generoi_metar();

			if (!tarkista_atis()) {
				atis::ok = false;
				pyyda_atis();
			}

			koska_metar += ohjelma::anna_asetus("koska_metar");
		}

		// Esc-nappi lopettaa pelin.
		if (ohjelma::lue_nappi(ohjelma::NAPPI_ESCAPE)) {
			loppu = true;
		}

		if (ohjelma::lue_nappi(ohjelma::NAPPI_F5)) {
			toiminto = SUUNTA;
		} else if (ohjelma::lue_nappi(ohjelma::NAPPI_F7)) {
			toiminto = NOPEUS;
		} else if (ohjelma::lue_nappi(ohjelma::NAPPI_F8)) {
			toiminto = KORKEUS;
		}

		lukija.lue_syote();

		if (ohjelma::lue_nappi(ohjelma::NAPPI_ENTER) && etsi_valittu_kone() >= 0) {
			koneet[etsi_valittu_kone()].reaktioaika = pelin_kello + apuvalineet::arvo_luku(4, 10);
			std::string tmp = lukija.anna_viesti();

			if (toiminto == KORKEUS) {
                if (tmp.length() == 2 || tmp.length() == 3) {
                    double luku = apuvalineet::luvuksi<double>(tmp) * 100;
                    tmp = apuvalineet::tekstiksi(luku);
                }
			}

			if (tmp == "ILS" && koneet[etsi_valittu_kone()].tyyppi == SAAPUVA) {
				toiminto = LAHESTYMIS;
			} else if (tmp == "DCT" && koneet[etsi_valittu_kone()].tyyppi == LAHTEVA) {
				toiminto = OIKOTIE;
			} else if (tmp == "HOLD") {
				toiminto = ODOTUS;
            } else if (tmp == "OFF") {
                toiminto = POIS;
			}

			koneet[etsi_valittu_kone()].ota_selvitys(tmp, toiminto);

			peli::lisaa_selvityksia();
			ohjelma::odota(150);

			lukija.tyhjenna();
			toiminto = TYHJA;
			peli::virheteksti = " ";
		}

		if (kasitellyt == ohjelma::anna_asetus("vaadittavat_kasitellyt")) {
			koska_uusi_kone = -1;
		}

		if (etsi_valittu_kone() < 0) {
			ohje = "Valitse kone klikkaamalla";
		} else {
			ohje = "Paina toimintonappulaa F5 F6 tai F8 ja anna komento";
		}

		tarkista_porrastus();

		while (pelin_kello + ajan_muutos <= ohjelma::sekunnit()) {
			pelin_kello += ajan_muutos;

			hoida_koneet();

			piirretty = false;
		}
		if (piirretty) {
			ohjelma::odota();
		} else {
			ohjelma::piirra_peli();

			piirretty = true;
		}
	}

	ohjelma::tyhjenna_syote();
	ohjelma::piirra_tilasto();

	for (unsigned int i = 0; i < ajat.size(); ++i) {
		std::clog << ajat[i].tunnus << " " << ajat[i].sisaan << " " << ajat[i].pois << " " << (ajat[i].pois - ajat[i].sisaan) << " " << ajat[i].selvitykset << std::endl;
	}

	if (loppu) {
		ohjelma::odota_nappi();
	}

	return 0;
}

void peli::lataa_tunnukset(std::string tunnukset) {
	std::clog << "peli::lataa_tunnukset(" << tunnukset << ")" << std::endl;
	std::string tmp;

	std::ifstream sisaan(tunnukset.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + tunnukset + " ei ole tai se ei aukea");
	}

	while (sisaan >> tmp) {
		peli::tunnukset.push_back(tmp);
	}
}

void peli::aseta_virhe(int virhe) {
	std::clog << "peli::aseta_virhe(" << virhe << ")" << std::endl;
	peli::virheteksti = " ";
	++peli::muut_virheet;
	std::ofstream ulos("virhedata.txt", std::ios::app);

	switch (virhe) {
		case VIRHE_KORKEUS_ALA:
			peli::virheteksti = "Korkeuden alaraja on " + apuvalineet::tekstiksi(ohjelma::anna_asetus("selvityskorkeus_ala")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian alhainen aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_KORKEUS_YLA:
			peli::virheteksti = "Korkeuden yläraja on " + apuvalineet::tekstiksi(ohjelma::anna_asetus("selvityskorkeus_yla")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian suuri aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISKORKEUS:
			peli::virheteksti = "Lähestymisen yläraja on " + apuvalineet::tekstiksi(ohjelma::anna_asetus("maks_lahestymiskorkeus")) + " jalkaa";
			ulos << "Koneella liian suuri korkeus lähestymiseen aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISNOPEUS:
			peli::virheteksti = "Lähestymisen maksiminopeus on " + apuvalineet::tekstiksi(ohjelma::anna_asetus("maks_lahestymisnopeus")) + " solmua";
			ulos << "Koneella liian suuri nopeus lähestymiseen aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISSUUNTA:
			peli::virheteksti ="Koneen suunnan tulee olla kiitotien suunnasta +- " + apuvalineet::tekstiksi(ohjelma::anna_asetus("lahestymiskulma"));
			ulos << "Koneen suunnan tulee olla +- " << ohjelma::anna_asetus("lahestymiskulma") << std::endl;
		case VIRHE_NOPEUS_ALA:
			peli::virheteksti = "Nopeuden alaraja on " + apuvalineet::tekstiksi(ohjelma::anna_asetus("selvitysnopeus_ala")) + " solmua";
			ulos << "Annettu selvitysnopeus on liian pieni aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_NOPEUS_YLA:
			peli::virheteksti = "Nopeuden yläraja on " + apuvalineet::tekstiksi(ohjelma::anna_asetus("selvitysnopeus_yla")) + " solmua";
			ulos << "Annettu selvitysnopeus on suuri pieni aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_PORRASTUS:
			peli::virheteksti = "Porrastuksen tulee olla pystysuuntaan " + apuvalineet::tekstiksi(ohjelma::anna_asetus("porrastus_pysty")) + " jalkaa ja vaakasuuntaan " + apuvalineet::tekstiksi(ohjelma::anna_asetus("porrastus_vaaka")) + " mailia";
			ulos << "Porrastusminimi alittui aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_OIKOTIE:
			peli::virheteksti = "Oikotien saa antaa vasta " + apuvalineet::tekstiksi(ohjelma::anna_asetus("oikotie")) + " jalassa";
			ulos << "Annettu oikotie liian aikaisin aikaan " << ohjelma::sekunnit() << std::endl;
			break;
		case VIRHE_ALUEELTA:
			peli::virheteksti = "Kone poistui alueeltasi";
			ulos << "Kone poistui alueeltasi aikaan " << ohjelma::sekunnit() << std::endl;
			break;
	}

	ulos.close();
}

void peli::luo_kone() {
	int j = apuvalineet::arvo_luku(10, 100) % 2;
	apuvalineet::piste paikka;
	bool odotus;
	std::string tunnus = generoi_tunnus();

	if (j == LAHTEVA) {
		double suunta = kentta.kiitotiet[atis::lahtokiitotie].suunta;

		if (!onko_vapaata()) {
			paikka = kentta.kiitotiet[atis::lahtokiitotie].odotuspiste;
			odotus = true;
		} else {
			paikka = kentta.kiitotiet[atis::lahtokiitotie].alkupiste;
			odotus = false;
		}

		koneet.push_back(lentokone(tunnus, paikka, kentta.korkeus, 0.0, suunta, LAHTEVA, odotus));
		koneet.back().ulosmenopiste = navipisteet[apuvalineet::arvo_luku(0, navipisteet.size())];
		koneet.back().polttoaine = apuvalineet::arvo_luku(8000, 25000);

		if (koneet.back().odotus) {
			odottavat.push_back(koneet.back());
		}

	} else {
		int i = apuvalineet::arvo_luku(0, navipisteet.size());

        koneet.push_back(lentokone(tunnus, navipisteet[i].paikka, navipisteet[i].lentokorkeus, navipisteet[i].lentonopeus, navipisteet[i].lentosuunta, SAAPUVA, false));
		koneet.back().polttoaine = apuvalineet::arvo_luku(3300, 5200);
	}

	tilasto tmp;

	tmp.tunnus = tunnus;
	tmp.sisaan = ohjelma::sekunnit();
	tmp.pois = 0.0;
	tmp.selvitykset = 0;

	ajat.push_back(tmp);
}

void peli::lataa_navipisteet(std::string nimi) {
	std::clog << "peli::lataa_navipisteet(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + nimi + " ei ole tai se ei aukea");
	}

	double _suunta, etaisyys, korkeus, nopeus, suunta;
	std::string _nimi;

	while (sisaan >> _nimi >> _suunta >> etaisyys >> korkeus >> nopeus >> suunta) {
		apuvalineet::piste paikka;

		paikka = apuvalineet::uusi_paikka(peli::kentta.paikka, _suunta, etaisyys);

		navipiste tmp(_nimi, paikka, korkeus, nopeus, suunta);

		navipisteet.push_back(tmp);
	}

	sisaan.close();
}

void peli::lataa_lahipisteet(std::string nimi) {
	std::clog << "peli::lataa_sisapisteet(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + nimi + " ei ole tai se ei aukea");
	}

	double suunta, etaisyys;
	std::string _nimi;

	while (sisaan >> _nimi >> suunta >> etaisyys) {
		apuvalineet::piste paikka;

		paikka = apuvalineet::uusi_paikka(kentta.paikka, suunta, etaisyys);

		navipiste tmp(_nimi, paikka);

		sisapisteet.push_back(tmp);
	}

	sisaan.close();
}

void peli::lataa_kentta(std::string kentta) {
	std::string paate = ".txt";
	std::string kansio = "kentat/";
	std::string tmp;
	std::clog << "peli::lataa_kiitotiet(" << kansio + kentta + paate << ")" << std::endl;
	tmp = kansio + kentta + paate;
	std::ifstream sisaan(tmp.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + tmp + " ei ole tai se ei aukea");
	}

	std::string nimi;
	double pituus, suunta, nousukorkeus, noususuunta;
	double alkupiste_suunta, alkupiste_etaisyys;

	sisaan >> peli::kentta.nimi >> peli::kentta.korkeus >> peli::kentta.paikka.x >> peli::kentta.paikka.y;

	while (sisaan >> nimi >> alkupiste_suunta >> alkupiste_etaisyys >> suunta >> pituus >> nousukorkeus >> noususuunta) {
		apuvalineet::piste alku = apuvalineet::uusi_paikka(peli::kentta.paikka, alkupiste_suunta, alkupiste_etaisyys);
		kiitotie tmpa(nimi, alku, pituus, suunta, nousukorkeus, noususuunta);

		peli::kentta.kiitotiet.push_back(tmpa);
	}

	tmp = kansio + kentta + "_pisteet" + paate;

	lataa_navipisteet(tmp);

	tmp = kansio + kentta + "_lahipisteet" + paate;

	lataa_lahipisteet(tmp);

	sisaan.close();
}

std::string peli::generoi_tunnus() {
	std::string tunniste = tunnukset[apuvalineet::arvo_luku(0, tunnukset.size())];
	std::string luku = apuvalineet::tekstiksi(apuvalineet::arvo_luku(0, 999));

	return tunniste+luku;
}

void peli::valitse_kone(const apuvalineet::piste& hiiri) {
	if (ohjelma::lue_hiiri()) {
		for (unsigned int i = 0; i < koneet.size(); ++i) {
			koneet[i].valittu = false;

			if (ohjelma::onko_alueella(hiiri, koneet[i].paikka)) {
				koneet[i].valittu = true;
			}
		}
	}
}

void peli::tarkista_porrastus() {
	bool virheita = false;
	std::vector <int> alittuu;

	for (unsigned int i = 0; i < koneet.size(); ++i) {
		for (unsigned int j = 0; j < koneet.size(); ++j) {
			if (i ==j) {
				continue;
			}

			if (ohjelma::onko_alueella(koneet[i].paikka, koneet[j].paikka, 1.5) && std::abs(koneet[i].korkeus - koneet[j].korkeus) < 1000 && (koneet[i].korkeus > 1000 && koneet[j].korkeus > 1000)) {
				alittuu.push_back(i);
				alittuu.push_back(j);

				if (koneet[i].onko_porrastus == true || koneet[j].onko_porrastus == true) {
					virheita = true;
				}
			}
		}
	}

	if (virheita) {
		++peli::porrastusvirheet;
		aseta_virhe(VIRHE_PORRASTUS);
	}

	for (unsigned int i = 0; i < koneet.size(); ++i) {
		koneet[i].onko_porrastus = true;
	}

	for (unsigned int i = 0; i < alittuu.size(); ++i) {
		koneet[alittuu[i]].onko_porrastus = false;
	}
}

void peli::hoida_koneet() {
	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (ohjelma::onko_alueella(koneet[i].paikka, koneet[i].kohde)) {
			if (koneet[i].reitti.size()) {
				navipiste tmp = koneet[i].anna_piste();
				koneet[i].reitti.pop();
				koneet[i].kohde = tmp.paikka;
			} else {
				koneet[i].kohde.x = 0;
				koneet[i].kohde.y = 0;
			}
		}

		if (koneet[i].odotus) {
			if (onko_vapaata()) {
				koneet[i].paikka = kentta.kiitotiet[atis::lahtokiitotie].alkupiste;
				koneet[i].odotus = false;
				odottavat.erase(odottavat.begin()+0);
			}
		}

		if (koneet[i].odotuskuvio > -1 && koneet[i].odotuskuvio < ohjelma::sekunnit()) {
			koneet[i].odotuskuvio += 120;
			koneet[i].muuta_selvityssuuntaa(koneet[i].suunta + 180, OIKEA);
		}

		if (koneet[i].paikka.x < 0 || koneet[i].paikka.x > ohjelma::anna_asetus("ruutu_leveys") || koneet[i].paikka.y < 0 || koneet[i].paikka.y > ohjelma::anna_asetus("ruutu_korkeus")) {
			aseta_virhe(VIRHE_ALUEELTA);
			poista_kone(i);
		}

		if (koneet[i].kohde.x > 0 && koneet[i].kohde.y > 0) {
			koneet[i].tarkista_suunta_kohteeseen();
		}

		if (koneet[i].laskubaana >= 0) {
			koneet[i].lahesty();

			if (koneet[i].nopeus < 4.0) {
				poista_kone(i);
				++kasitellyt;
			}
		}

		if (koneet[i].tyyppi == peli::LAHTEVA) {
			if (ohjelma::onko_alueella(koneet[i].paikka, koneet[i].ulosmenopiste.paikka)) {
				poista_kone(i);
				++kasitellyt;
			} else if (ohjelma::onko_alueella(koneet[i].paikka, koneet[i].kohde)) {
				if (koneet[i].reitti.size()) {
					koneet[i].aseta_navipiste(koneet[i].anna_piste().paikka);
				}
			}
		}

		if (koneet[i].tyyppi == peli::LAHTEVA && koneet[i].korkeus < 1200 && koneet[i].odotus == false) {
			if (koneet[i].nopeus == 0) {
				koneet[i].muuta_selvitysnopeutta(ohjelma::anna_asetus("alkunopeus"));
			}

			if (koneet[i].nopeus > 150) {
				koneet[i].muuta_selvityskorkeutta(peli::kentta.kiitotiet[peli::atis::lahtokiitotie].alkunousukorkeus);
				koneet[i].muuta_selvitysnopeutta(ohjelma::anna_asetus("alkunousunopeus"));
				koneet[i].muuta_selvityssuuntaa(peli::kentta.kiitotiet[peli::atis::lahtokiitotie].alkunoususuunta);
			}
		}

		koneet[i].muuta_tilaa(ajan_muutos);
		koneet[i].liiku(ajan_muutos);

		if (koneet[i].polttoaine < ohjelma::anna_asetus("minimi_polttoaine")) {
			ohje = "Koneella " + koneet[i].kutsutunnus + " on polttoaine lopussa";
		}
	}
}

void peli::poista_kone(int kone) {
	std::vector <tilasto>::iterator lisattava = std::find(ajat.begin(), ajat.end(), koneet[kone].kutsutunnus);
	lisattava->pois = ohjelma::sekunnit();

	koneet.erase(koneet.begin()+kone);
}

void peli::lisaa_selvityksia() {
	int valittu = etsi_valittu_kone();
	std::vector <tilasto>::iterator lisattava = std::find(ajat.begin(), ajat.end(), koneet[valittu].kutsutunnus);
	++lisattava->selvitykset;
}

int peli::etsi_valittu_kone() {
	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (koneet[i].valittu) {
			return i;
		}
	}

	return -1;
}

void peli::pyyda_atis() {
	std::clog << "peli::pyyda_atis" << std::endl;
	syotteenluku lukija;
	peli::atis::lue_paineet("data/painerajat.txt");
	int toiminto = LAHTO;

	while (atis::ok == false) {
		lukija.lue_syote();
		ohjelma::odota(20);

		if (lukija.anna_viesti().length() > 1 && ohjelma::lue_nappi(ohjelma::NAPPI_ENTER)) {
			std::vector <kiitotie>::iterator tmp;
			size_t index;

			if (ohjelma::lue_nappi(ohjelma::NAPPI_F5)) {
				toiminto = LAHTO;
			} else if (ohjelma::lue_nappi(ohjelma::NAPPI_F7)) {
				toiminto = LASKU;
			} else if (ohjelma::lue_nappi(ohjelma::NAPPI_F8)) {
				toiminto = SIIRTOPINTA;
			}

			switch (toiminto) {
				case LAHTO:
					tmp = std::find(kentta.kiitotiet.begin(), kentta.kiitotiet.end(), lukija.anna_viesti());
					index = std::distance(kentta.kiitotiet.begin(), tmp);

					if (tmp != kentta.kiitotiet.end()) {
						atis::lahtokiitotie = index;
						atis::lahto = lukija.anna_viesti();
						lukija.tyhjenna();
					}
					toiminto = LASKU;
					break;
				case LASKU:
					tmp = std::find(kentta.kiitotiet.begin(), kentta.kiitotiet.end(), lukija.anna_viesti());
					index = std::distance(kentta.kiitotiet.begin(), tmp);

					if (tmp != kentta.kiitotiet.end()) {
						atis::laskukiitotie = index;
						atis::lasku = lukija.anna_viesti();
						lukija.tyhjenna();
					}
					toiminto = SIIRTOPINTA;
					break;
				case SIIRTOPINTA:
					atis::siirtopinta = apuvalineet::luvuksi<int>(lukija.anna_viesti());
					lukija.tyhjenna();
					break;
			}
		}

		bool siirto_ok = false;
		bool lahto_ok = false;
		bool lasku_ok = false;

		ohjelma::piirra_atis(toiminto);
		if (atis::lahtokiitotie > -1 && atis::laskukiitotie > -1 && atis::siirtopinta > -1) {
			peli::ohje = "Tiedot syötetty tarkistetaan onko oikein";

			double vastakomponentti_lahto = std::cos(apuvalineet::deg2rad(metar::tuuli) - apuvalineet::deg2rad(kentta.kiitotiet[atis::lahtokiitotie].suunta)) * metar::voimakkuus;
			double vastakomponentti_lasku = std::cos(apuvalineet::deg2rad(metar::tuuli) - apuvalineet::deg2rad(kentta.kiitotiet[atis::laskukiitotie].suunta)) * metar::voimakkuus;

			double siirtopinta = peli::atis::etsi_siirtopinta(peli::metar::paine);

			double max_vasta = 0;
			double vasta;

			for (unsigned int i = 0; i < kentta.kiitotiet.size(); ++i) {
				vasta = std::cos(apuvalineet::deg2rad(metar::tuuli) - apuvalineet::deg2rad(kentta.kiitotiet[i].suunta)) * metar::voimakkuus;

				if (vasta > max_vasta) {
					max_vasta = vasta;
				}
			}

			if (siirtopinta == atis::siirtopinta) {
				siirto_ok = true;
			} else {
				ohje = "Siirtopinta väärin tulisi olla " + apuvalineet::tekstiksi(siirtopinta);
				toiminto = SIIRTOPINTA;
			}

			if (vastakomponentti_lahto > 0) {
				lahto_ok = true;
			} else {
				ohje = "Lähtökiitotie valittu väärin";
				toiminto = LAHTO;
			}

			if (vastakomponentti_lasku > 0) {
				lasku_ok = true;
			} else {
				ohje = "Laskukiitotie valittu väärin";
				toiminto = LASKU;
			}
		}

		if (siirto_ok && lahto_ok && lasku_ok) {
			atis::ok = true;
			atis::siirtokorkeus = 5000;
		}
	}
}

static void peli::generoi_metar() {
	metar::tuuli 		= apuvalineet::pyorista(apuvalineet::arvo_luku(0, 360), 5);
	metar::voimakkuus 	= apuvalineet::arvo_luku(2, 22);
	metar::paine 		= apuvalineet::arvo_luku(950, 1060);
	metar::nakyvyys		= apuvalineet::arvo_luku(1200, 9999);
	metar::lampotila	= apuvalineet::arvo_luku(-18, 27);
	metar::ilmankosteus	= apuvalineet::arvo_luku(50, 100);
	metar::kastepiste	= metar::lampotila - ((100 - metar::ilmankosteus) / 5);
}

static bool peli::onko_vapaata() {
	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (koneet[i].korkeus < (kentta.korkeus + ohjelma::anna_asetus("porrastus_pysty")) && koneet[i].odotus == false) {
			return false;
		}
	}

	return true;
}

static bool peli::tarkista_atis() {
	peli::atis::lue_paineet("data/painerajat.txt");

	double vasta_lahto = std::cos(std::abs(peli::kentta.kiitotiet[peli::atis::lahtokiitotie].suunta - peli::metar::tuuli));
	double vasta_lasku = std::cos(std::abs(peli::kentta.kiitotiet[peli::atis::laskukiitotie].suunta - peli::metar::tuuli));
	double siirtopinta = peli::atis::etsi_siirtopinta(peli::metar::paine);

	if (vasta_lahto < 0 || vasta_lasku < 0 || siirtopinta != atis::siirtopinta) {
		return false;
	}

	return true;
}

void peli::atis::lue_paineet(std::string nimi) {
	std::clog << "peli::atis::lue_paineet(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + nimi + " ei löydy tai se ei aukea");
	}

	double ala, yla, pinta;

	while (sisaan >> ala >> yla >> pinta) {
		peli::atis::paine tmp;
		tmp.alaraja = ala;
		tmp.ylaraja = yla;
		tmp.siirtopinta = pinta;

		peli::atis::paineet.push_back(tmp);
	}

	sisaan.close();
}

double peli::atis::etsi_siirtopinta(double paine) {
	for (unsigned int i = 0; i < peli::atis::paineet.size(); ++i) {

		if (paine >= peli::atis::paineet[i].alaraja && paine < peli::atis::paineet[i].ylaraja) {
			return peli::atis::paineet[i].siirtopinta;
		}
	}

	return 0.0;
}
