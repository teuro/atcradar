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

// Pelin pääfunktio.
int PeliController::aja() {
	std::clog << "peli::aja()" << std::endl;

	//ohjelma::lataa_asetukset("asetukset.ini");
	peli.lataa_tunnukset("data/tunnukset.txt");

    peli.lataa_kentta("EFRO");

	std::srand(std::time(NULL));

    peli.generoi_metar();
	bool loppu = false, piirretty = false;

    pyyda_atis();

	// Nollataan kello.
	float pelin_kello = ohjelma.sekunnit(true);

	ohjelma.tyhjenna_syote();

	apuvalineet::piste hiiri;
	syotteenluku lukija;
	peli.koska_metar = Asetukset::anna_asetus("koska_metar");

	std::vector <ajastin> ajastimet;

	ajastimet.push_back(ajastin("metar", Asetukset::anna_asetus("koska_metar"), 0));

	peli.luo_kone(ohjelma);
	peli.luo_kone(ohjelma);

	while (!loppu) {
		peli.alku = ohjelma.sekunnit();
		hiiri = ohjelma.anna_hiiri();

		peli.valitse_kone(hiiri);

		if (peli.porrastusvirheet >= Asetukset::anna_asetus("maks_porrastusvirhe")) {
			std::clog << "Porrastusvirheet täynnä" << std::endl;
			loppu = true;
		}

		if (peli.alku == peli.koska_uusi_kone) {
			if (peli.koneet.size() < Asetukset::anna_asetus("maks_konemaara")) {
				peli.luo_kone(ohjelma);
                peli.koska_uusi_kone += apuvalineet::arvo_luku(Asetukset::anna_asetus("koska_uusi_ala"), Asetukset::anna_asetus("koska_uusi_yla"));
			}

			peli.koska_uusi_kone += 15;

			std::clog << "Seuraava kone luodaan " << (peli.koska_uusi_kone - peli.alku) << " sekunnin kuluttua" << std::endl;
		}

		if (peli.alku == peli.koska_metar) {
			peli.generoi_metar();

			if (!peli.tarkista_atis()) {
				peli.atis.ok = false;
				pyyda_atis();
			}

            peli.koska_metar += Asetukset::anna_asetus("koska_metar");
		}

		// Esc-nappi lopettaa pelin.
		if (ohjelma.lue_nappi(Ohjelma::NAPPI_ESCAPE)) {
			loppu = true;
		}

		if (ohjelma.lue_nappi(Ohjelma::NAPPI_F5)) {
			peli.toiminto = Peli::SUUNTA;
		} else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F7)) {
            peli.toiminto = Peli::NOPEUS;
		} else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F8)) {
            peli.toiminto = Peli::KORKEUS;
		}

		peli.syote = lukija.lue_syote();

		if (ohjelma.lue_nappi(Ohjelma::NAPPI_ENTER) && peli.etsi_valittu_kone() >= 0) {
			peli.koneet[peli.etsi_valittu_kone()].reaktioaika = pelin_kello + apuvalineet::arvo_luku(4, 10);
			std::string tmp = lukija.anna_viesti();

            if (peli.toiminto == Peli::KORKEUS) {
                if (tmp.length() == 2 || tmp.length() == 3) {
                    double luku = apuvalineet::luvuksi<double>(tmp) * 100;
                    tmp = apuvalineet::tekstiksi(luku);
                }
			}

            if (tmp == "ILS" && peli.koneet[peli.etsi_valittu_kone()].tyyppi == Peli::SAAPUVA) {
                peli.toiminto = Peli::LAHESTYMIS;
            }
            else if (tmp == "DCT" && peli.koneet[peli.etsi_valittu_kone()].tyyppi == Peli::LAHTEVA) {
                peli.toiminto = Peli::OIKOTIE;
			} else if (tmp == "HOLD") {
                peli.toiminto = Peli::ODOTUS;
            } else if (tmp == "OFF") {
                peli.toiminto = Peli::POIS;
			}

			peli.koneet[peli.etsi_valittu_kone()].ota_selvitys(tmp, peli.toiminto);

			peli.lisaa_selvityksia();
			ohjelma.odota(150);

			lukija.tyhjenna();
            peli.toiminto = Peli::TYHJA;
			peli.virheteksti = " ";
		}

        if (peli.kasitellyt == Asetukset::anna_asetus("vaadittavat_kasitellyt")) {
			peli.koska_uusi_kone = -1;
		}

		if (peli.etsi_valittu_kone() < 0) {
			peli.ohje = "Valitse kone klikkaamalla";
		} else {
			peli.ohje = "Paina toimintonappulaa F5 F6 tai F8 ja anna komento";
		}

		peli.tarkista_porrastus();

		while (pelin_kello + peli.ajan_muutos <= ohjelma.sekunnit()) {
			pelin_kello += peli.ajan_muutos;

			peli.hoida_koneet();

			piirretty = false;
		}
		if (piirretty) {
			ohjelma.odota();
		} else {
			view.piirra_peli();

			piirretty = true;
		}
	}

	ohjelma.tyhjenna_syote();
	view.piirra_tilasto();

	for (unsigned int i = 0; i < peli.ajat.size(); ++i) {
		std::clog << peli.ajat[i].tunnus << " " << peli.ajat[i].sisaan << " " << peli.ajat[i].pois << " " << (peli.ajat[i].pois - peli.ajat[i].sisaan) << " " << peli.ajat[i].selvitykset << std::endl;
	}

	if (loppu) {
		ohjelma.odota_nappi();
	}

	return 0;
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
            virheteksti = "Korkeuden yläraja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvityskorkeus_yla")) + " jalkaa";
			ulos << "Annettu selvityskorkeus liian suuri aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISKORKEUS:
			virheteksti = "Lähestymisen yläraja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("maks_lahestymiskorkeus")) + " jalkaa";
			ulos << "Koneella liian suuri korkeus lähestymiseen aikaan " << ohjelma.sekunnit() << " " << Asetukset::anna_asetus("maks_lahestymiskorkeus") << std::endl;
			break;
		case VIRHE_LAHESTYMISNOPEUS:
			virheteksti = "Lähestymisen maksiminopeus on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("maks_lahestymisnopeus")) + " solmua";
			ulos << "Koneella liian suuri nopeus lähestymiseen aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_LAHESTYMISSUUNTA:
			virheteksti ="Koneen suunnan tulee olla kiitotien suunnasta +- " + apuvalineet::tekstiksi(Asetukset::anna_asetus("lahestymiskulma"));
			ulos << "Koneen suunnan tulee olla +- " << Asetukset::anna_asetus("lahestymiskulma") << std::endl;
		case VIRHE_NOPEUS_ALA:
			virheteksti = "Nopeuden alaraja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvitysnopeus_ala")) + " solmua";
			ulos << "Annettu selvitysnopeus on liian pieni aikaan " << ohjelma.sekunnit() << std::endl;
			break;
		case VIRHE_NOPEUS_YLA:
			virheteksti = "Nopeuden yläraja on " + apuvalineet::tekstiksi(Asetukset::anna_asetus("selvitysnopeus_yla")) + " solmua";
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

void PeliController::pyyda_atis() {
    Peli::Atis &atis = peli.atis;
	syotteenluku lukija;
	atis.lue_paineet("data/painerajat.txt");
	int toiminto = Peli::LAHTO;

	while (atis.ok == false) {
        peli.syote = lukija.lue_syote();
		ohjelma.odota(20);

		if (lukija.anna_viesti().length() > 1 && ohjelma.lue_nappi(Ohjelma::NAPPI_ENTER)) {

			std::vector <kiitotie>::iterator tmp;
			size_t index;

			if (ohjelma.lue_nappi(Ohjelma::NAPPI_F5)) {
                toiminto = Peli::LAHTO;
			} else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F7)) {
                toiminto = Peli::LASKU;
			} else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F8)) {
                toiminto = Peli::SIIRTOPINTA;
			}

            switch (toiminto) {
            case Peli::LAHTO:
					tmp = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), lukija.anna_viesti());
					index = std::distance(peli.kentta.kiitotiet.begin(), tmp);

					if (tmp != peli.kentta.kiitotiet.end()) {
						atis.lahtokiitotie = index;
						atis.lahto = lukija.anna_viesti();
						lukija.tyhjenna();
					}
                    toiminto = Peli::LASKU;
					break;
            case Peli::LASKU:
					tmp = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), lukija.anna_viesti());
					index = std::distance(peli.kentta.kiitotiet.begin(), tmp);

					if (tmp != peli.kentta.kiitotiet.end()) {
						atis.laskukiitotie = index;
						atis.lasku = lukija.anna_viesti();
						lukija.tyhjenna();
					}
                    toiminto = Peli::SIIRTOPINTA;
					break;
            case Peli::SIIRTOPINTA:
					atis.siirtopinta = apuvalineet::luvuksi<int>(lukija.anna_viesti());
					lukija.tyhjenna();
					break;
			}
		}

		bool siirto_ok = false;
		bool lahto_ok = false;
		bool lasku_ok = false;

		view.piirra_atis(toiminto);

		if (atis.lahtokiitotie > -1 && atis.laskukiitotie > -1 && atis.siirtopinta > -1) {
			peli.ohje = "Tiedot syötetty tarkistetaan onko oikein";

			double vastakomponentti_lahto = std::cos(apuvalineet::deg2rad(peli.metar.tuuli) - apuvalineet::deg2rad(peli.kentta.kiitotiet[atis.lahtokiitotie].suunta)) * peli.metar.voimakkuus;
			double vastakomponentti_lasku = std::cos(apuvalineet::deg2rad(peli.metar.tuuli) - apuvalineet::deg2rad(peli.kentta.kiitotiet[atis.laskukiitotie].suunta)) * peli.metar.voimakkuus;

			double siirtopinta = atis.etsi_siirtopinta(peli.metar.paine);

			double max_vasta = 0;
			double vasta;

			for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
				vasta = std::cos(apuvalineet::deg2rad(peli.metar.tuuli) - apuvalineet::deg2rad(peli.kentta.kiitotiet[i].suunta)) * peli.metar.voimakkuus;

				if (vasta > max_vasta) {
					max_vasta = vasta;
				}
			}

			if (siirtopinta == atis.siirtopinta) {
				siirto_ok = true;
			} else {
				peli.ohje = "Siirtopinta väärin tulisi olla " + apuvalineet::tekstiksi(siirtopinta);
				toiminto = Peli::SIIRTOPINTA;
			}

			if (vastakomponentti_lahto > 0) {
				lahto_ok = true;
			} else {
				peli.ohje = "Lähtökiitotie valittu väärin";
                toiminto = Peli::LAHTO;
			}

			if (vastakomponentti_lasku > 0) {
				lasku_ok = true;
			} else {
				peli.ohje = "Laskukiitotie valittu väärin";
                toiminto = Peli::LASKU;
			}
		}

		if (siirto_ok && lahto_ok && lasku_ok) {
			atis.ok = true;
			atis.siirtokorkeus = 5000;
		}
	}
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
		throw std::runtime_error("Tiedostoa " + nimi + " ei löydy tai se ei aukea");
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
