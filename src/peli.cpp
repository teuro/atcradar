#include "asetukset.h"
#include "peli.hpp"
#include "ohjelma.hpp"
#include "lukija.hpp"
#include "ajastin.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

<<<<<<< HEAD
struct selvitys {
	int kone_id;
	std::string selvitys;
	int toiminto;
	float aika;
};

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
	std::vector <selvitys> selvitykset;
	
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

// Pelin p‰‰funktio.
int peli::aja() {
	std::clog << "peli::aja()" << std::endl;

	//ohjelma::lataa_asetukset("asetukset.ini");
	lataa_tunnukset("data/tunnukset.txt");

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
			std::clog << "Porrastusvirheet t‰ynn‰" << std::endl;
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
			
			selvitys tmp_selvitys;
			tmp_selvitys.kone_id = etsi_valittu_kone();
			tmp_selvitys.selvitys = tmp;
			tmp_selvitys.toiminto = toiminto;
			tmp_selvitys.aika = ohjelma::sekunnit() + apuvalineet::arvo_luku(3, 8);
		
			selvitykset.push_back(tmp_selvitys);

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
		
		for (unsigned int k = 0; k < selvitykset.size(); ++k) {
			if ((int)selvitykset[k].aika == (int)ohjelma::sekunnit()) {
				koneet[selvitykset[k].kone_id].ota_selvitys(selvitykset[k].selvitys, selvitykset[k].toiminto);
				selvitykset.erase(selvitykset.begin()+k);
			}
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
=======
void Peli::lataa_tunnukset(std::string tunnukset) {
>>>>>>> 666aafec758181748575d76a3a5154ba0e3f75b9
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
	std::clog << "peli::aseta_virhe(" << virhe << ")" << std::endl;
	virheteksti = " ";
	++muut_virheet;
	std::ofstream ulos("virhedata.txt", std::ios::app);

	switch (virhe) {
		case VIRHE_KORKEUS_ALA:
            virheteksti = "Korkeuden alaraja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvityskorkeus_ala")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian alhainen aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_KORKEUS_YLA:
            virheteksti = "Korkeuden yl‰raja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvityskorkeus_yla")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian suuri aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISKORKEUS:
			virheteksti = "L‰hestymisen yl‰raja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("maks_lahestymiskorkeus")) + " jalkaa";
			ulos << "Koneella liian suuri korkeus l‰hestymiseen aikaan " << ohjelma.sekunnit() << " " << Asetukset::anna_asetus("maks_lahestymiskorkeus") << std::endl;
			break;
		case VIRHE_LAHESTYMISNOPEUS:
			virheteksti = "L‰hestymisen maksiminopeus on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("maks_lahestymisnopeus")) + " solmua";
			ulos << "Koneella liian suuri nopeus l‰hestymiseen aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISSUUNTA:
			virheteksti ="Koneen suunnan tulee olla kiitotien suunnasta +- " + apuvalineet::tekstiksi(Asetukset::anna_asetus("lahestymiskulma"));
			ulos << "Koneen suunnan tulee olla +- " << Asetukset::anna_asetus("lahestymiskulma") << std::endl;
		case VIRHE_NOPEUS_ALA:
			virheteksti = "Nopeuden alaraja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvitysnopeus_ala")) + " solmua";
			ulos << "Annettu selvitysnopeus on liian pieni aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_NOPEUS_YLA:
			virheteksti = "Nopeuden yl‰raja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvitysnopeus_yla")) + " solmua";
			ulos << "Annettu selvitysnopeus on suuri pieni aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_PORRASTUS:
			virheteksti = "Porrastuksen tulee olla pystysuuntaan " + apuvalineet::tekstiksi(Asetukset::anna_asetus("porrastus_pysty")) + " jalkaa ja vaakasuuntaan " + apuvalineet::tekstiksi(Asetukset::anna_asetus("porrastus_vaaka")) + " mailia";
			ulos << "Porrastusminimi alittui aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_OIKOTIE:
			virheteksti = "Oikotien saa antaa vasta " + apuvalineet::tekstiksi(Asetukset::anna_asetus("oikotie")) + " jalassa";
			ulos << "Annettu oikotie liian aikaisin aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_ALUEELTA:
			virheteksti = "Kone poistui alueeltasi";
			ulos << "Kone poistui alueeltasi aikaan " << ohjelma.sekunnit() << std::endl;
			break;
	}

	ulos.close();
}

void Peli::luo_kone(Ohjelma& ohjelma) {
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

		koneet.push_back(lentokone(*this, ohjelma, tunnus, paikka, kentta.korkeus, 0.0, suunta, LAHTEVA, odotus));
		koneet.back().ulosmenopiste = navipisteet[apuvalineet::arvo_luku(0, navipisteet.size())];
		koneet.back().polttoaine = apuvalineet::arvo_luku(8000, 25000);

		if (koneet.back().odotus) {
			odottavat.push_back(koneet.back());
		}

	} else {
		int i = apuvalineet::arvo_luku(0, navipisteet.size());

        koneet.push_back(lentokone(*this, ohjelma, tunnus, navipisteet[i].paikka, navipisteet[i].lentokorkeus, navipisteet[i].lentonopeus, navipisteet[i].lentosuunta, SAAPUVA, false));
		koneet.back().polttoaine = apuvalineet::arvo_luku(3300, 5200);
	}

	tilasto tmp;

	tmp.tunnus = tunnus;
	tmp.sisaan = ohjelma.sekunnit();
	tmp.pois = 0.0;
	tmp.selvitykset = 0;

	ajat.push_back(tmp);
}

void Peli::lataa_navipisteet(std::string nimi) {
	std::clog << "peli::lataa_navipisteet(" << nimi << ")" << std::endl;
	std::ifstream sisaan(nimi.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + nimi + " ei ole tai se ei aukea");
	}

	double _suunta, etaisyys, korkeus, nopeus, suunta;
	std::string _nimi;

	while (sisaan >> _nimi >> _suunta >> etaisyys >> korkeus >> nopeus >> suunta) {
		apuvalineet::piste paikka;

		paikka = apuvalineet::uusi_paikka(kentta.paikka, _suunta, etaisyys);

		navipiste tmp(_nimi, paikka, korkeus, nopeus, suunta);

		navipisteet.push_back(tmp);
	}

	sisaan.close();
}

void Peli::lataa_lahipisteet(std::string nimi) {
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

void Peli::lataa_kentta(std::string kenttaNimi) {
	std::string paate = ".txt";
	std::string kansio = "kentat/";
	std::string tmp;
    std::clog << "peli::lataa_kiitotiet(" << kansio + kenttaNimi + paate << ")" << std::endl;
    tmp = kansio + kenttaNimi + paate;
	std::ifstream sisaan(tmp.c_str(), std::ios::in);

	if (!sisaan) {
		throw std::runtime_error("Tiedostoa " + tmp + " ei ole tai se ei aukea");
	}

	std::string nimi;
	double pituus, suunta, nousukorkeus, noususuunta;
	double alkupiste_suunta, alkupiste_etaisyys;

	sisaan >> kentta.nimi >> kentta.korkeus >> kentta.paikka.x >> kentta.paikka.y;

	while (sisaan >> nimi >> alkupiste_suunta >> alkupiste_etaisyys >> suunta >> pituus >> nousukorkeus >> noususuunta) {
		apuvalineet::piste alku = apuvalineet::uusi_paikka(kentta.paikka, alkupiste_suunta, alkupiste_etaisyys);
		kiitotie tmpa(nimi, alku, pituus, suunta, nousukorkeus, noususuunta);

		kentta.kiitotiet.push_back(tmpa);
	}

    tmp = kansio + kenttaNimi + "_pisteet" + paate;

	lataa_navipisteet(tmp);

    tmp = kansio + kenttaNimi + "_lahipisteet" + paate;

	lataa_lahipisteet(tmp);

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
	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (ohjelma.onko_alueella(koneet[i].paikka, koneet[i].kohde.paikka)) {
			if (koneet[i].reitti.size()) {
				navipiste tmp = koneet[i].anna_piste();
				koneet[i].reitti.pop();
				koneet[i].kohde = tmp;
			} else {
				koneet[i].kohde.paikka.x = 0;
				koneet[i].kohde.paikka.y = 0;
			}
		}

		if (koneet[i].odotus) {
			if (onko_vapaata()) {
				koneet[i].paikka = kentta.kiitotiet[atis.lahtokiitotie].alkupiste;
				koneet[i].odotus = false;
				odottavat.erase(odottavat.begin()+0);
			}
		}

		if (koneet[i].odotuskuvio > -1 && koneet[i].odotuskuvio < ohjelma.sekunnit()) {
			koneet[i].odotuskuvio += 120;
			koneet[i].muuta_selvityssuuntaa(koneet[i].suunta + 180, OIKEA);
		}

		if (koneet[i].paikka.x < 0 || koneet[i].paikka.x > Asetukset::anna_asetus("ruutu_leveys") || koneet[i].paikka.y < 0 || koneet[i].paikka.y > Asetukset::anna_asetus("ruutu_korkeus")) {
			aseta_virhe(VIRHE_ALUEELTA);
			poista_kone(i);
		}

		if (koneet[i].kohde.paikka.x > 0 && koneet[i].kohde.paikka.y > 0) {
			koneet[i].tarkista_suunta_kohteeseen();
		}

		if (koneet[i].laskubaana >= 0) {
			koneet[i].lahesty();

			if (koneet[i].nopeus < 4.0) {
				poista_kone(i);
				++kasitellyt;
			}
		}

		if (koneet[i].tyyppi == Peli::LAHTEVA) {
			if (ohjelma.onko_alueella(koneet[i].paikka, koneet[i].ulosmenopiste.paikka)) {
				poista_kone(i);
				++kasitellyt;
			} else if (ohjelma.onko_alueella(koneet[i].paikka, koneet[i].kohde.paikka)) {
				if (koneet[i].reitti.size()) {
					koneet[i].aseta_navipiste(koneet[i].anna_piste());
				}
			}
		}

		if (koneet[i].tyyppi == Peli::LAHTEVA && koneet[i].korkeus < 1200 && koneet[i].odotus == false) {
			if (koneet[i].nopeus == 0) {
				koneet[i].muuta_selvitysnopeutta(Asetukset::anna_asetus("alkunopeus"));
			}

			if (koneet[i].nopeus > 150) {
				koneet[i].muuta_selvityskorkeutta(kentta.kiitotiet[atis.lahtokiitotie].alkunousukorkeus);
				koneet[i].muuta_selvitysnopeutta(Asetukset::anna_asetus("alkunousunopeus"));
				koneet[i].muuta_selvityssuuntaa(kentta.kiitotiet[atis.lahtokiitotie].alkunoususuunta);
			}
		}

		koneet[i].muuta_tilaa(ajan_muutos);
		koneet[i].liiku(ajan_muutos);

		if (koneet[i].polttoaine < Asetukset::anna_asetus("minimi_polttoaine")) {
			ohje = "Koneella " + koneet[i].kutsutunnus + " on polttoaine lopussa";
		}
	}
}

void Peli::poista_kone(int kone) {
	std::vector <tilasto>::iterator lisattava = std::find(ajat.begin(), ajat.end(), koneet[kone].kutsutunnus);
	lisattava->pois = ohjelma.sekunnit();

	koneet.erase(koneet.begin()+kone);
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

void Peli::generoi_metar() {
	metar.tuuli 		= apuvalineet::pyorista(apuvalineet::arvo_luku(0, 360), 5);
	metar.voimakkuus 	= apuvalineet::arvo_luku(2, 22);
	metar.paine 		= apuvalineet::arvo_luku(950, 1060);
	metar.nakyvyys		= apuvalineet::arvo_luku(1200, 9999);
	metar.lampotila	= apuvalineet::arvo_luku(-18, 27);
	metar.ilmankosteus	= apuvalineet::arvo_luku(50, 100);
	metar.kastepiste	= metar.lampotila - ((100 - metar.ilmankosteus) / 5);
}

bool Peli::onko_vapaata() {
	for (unsigned int i = 0; i < koneet.size(); ++i) {
		if (koneet[i].korkeus < (kentta.korkeus + Asetukset::anna_asetus("porrastus_pysty")) && koneet[i].odotus == false) {
			return false;
		}
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
		throw std::runtime_error("Tiedostoa " + nimi + " ei lˆydy tai se ei aukea");
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
