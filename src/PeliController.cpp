#include "asetukset.h"
#include "peli.hpp"
#include "pelicontroller.hpp"
#include "ohjelma.hpp"
#include "ajastin.hpp"
#include "kieli.hpp"
#include "atiscontroller.hpp"
#include "atisview.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Pelin p‰‰funktio.
int PeliController::aja() {
	std::clog << "pelicontroller::aja()" << std::endl;

	std::srand((unsigned int)std::time(NULL));

	pyyda_atis();

	// Nollataan kello.
	double pelin_kello = ohjelma.sekunnit(true);

	// TODO: Tarvitaanko t‰t‰ tyhjennyst‰?
	ohjelma.tyhjenna_syote();

	std::vector <ajastin> ajastimet;
	ajastimet.push_back(ajastin("metar", asetukset.anna_asetus("koska_metar"), 0));

	peli.luo_kone();
	peli.luo_kone();

	bool loppu = false;
	while (!loppu) {
		int alku = ohjelma.sekunnit();
		apuvalineet::piste hiiri = ohjelma.anna_hiiri();

		peli.valitse_kone(hiiri);

		if (peli.porrastusvirheet >= asetukset.anna_asetus("maks_porrastusvirhe")) {
			std::clog << kieli.anna_teksti(Kieli::TEKSTI_PORRASTUSVIRHEET) << std::endl;
			loppu = true;
		}

		if (alku == peli.koska_uusi_kone) {
			if (peli.koneet.size() < asetukset.anna_asetus("maks_konemaara")) {
				peli.luo_kone();
				peli.koska_uusi_kone += apuvalineet::arvo_luku(asetukset.anna_asetus("koska_uusi_ala"), asetukset.anna_asetus("koska_uusi_yla"));
			}

			std::clog << kieli.anna_teksti(Kieli::TEKSTI_UUSI_KONE_TULEE) << (peli.koska_uusi_kone - alku) << kieli.anna_teksti(Kieli::TEKSTI_SEKUNNIT) << std::endl;
		}

		if (alku == peli.koska_metar) {
			peli.generoi_metar();

			if (!peli.tarkista_atis()) {
				peli.atis.ok = false;
				pyyda_atis();
			}

			peli.koska_metar += asetukset.anna_asetus("koska_metar");
		}

		// Esc-nappi lopettaa pelin.
		if (ohjelma.lue_nappi(Ohjelma::NAPPI_ESCAPE)) {
			loppu = true;
		}

		if (ohjelma.lue_nappi(Ohjelma::NAPPI_F5)) {
			peli.toiminto = Peli::SUUNTA;
		}
		else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F7)) {
			peli.toiminto = Peli::NOPEUS;
		}
		else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F8)) {
			peli.toiminto = Peli::KORKEUS;
		}

		peli.syote = ohjelma.lue_syote(); //lukija.lue_syote();

		int valittu_kone = peli.etsi_valittu_kone();
		if (ohjelma.lue_nappi(Ohjelma::NAPPI_ENTER) && valittu_kone >= 0) {
			// TODO: Fix reaktioaika
//			peli.koneet[peli.etsi_valittu_kone()].reaktioaika = pelin_kello + apuvalineet::arvo_luku(4, 10);
			std::string komento = ohjelma.anna_viesti();// lukija.anna_viesti();

			if (peli.toiminto == Peli::KORKEUS) {
				if (komento.length() == 2 || komento.length() == 3) {
					double luku = apuvalineet::luvuksi<double>(komento) * 100;
					komento = apuvalineet::tekstiksi(luku);
				}
			}

			if ((komento == "ILS" || komento == "ils") && peli.koneet[valittu_kone].tyyppi == Peli::SAAPUVA) {
				peli.toiminto = Peli::LAHESTYMIS;
			}
			else if ((komento == "DCT" || komento == "dct") && peli.koneet[valittu_kone].tyyppi == Peli::LAHTEVA) {
				peli.toiminto = Peli::OIKOTIE;
			}
			else if (komento == "HOLD" || komento == "hold") {
				peli.toiminto = Peli::ODOTUS;
			}
			else if (komento == "OFF" || komento == "off") {
				peli.toiminto = Peli::POIS;
			}

			int kaarto = 0;

			if ((komento.substr(0, 1) == "V" || komento.substr(0, 1) == "v") && komento.length() < 5) {
				kaarto = VASEN;
				komento = komento.substr(1, std::string::npos);
			} else if ((komento.substr(0, 1) == "O" || komento.substr(0, 1) == "o") && komento.length() < 5) {
				kaarto = OIKEA;
				komento = komento.substr(1, std::string::npos);
			} else {
				kaarto = VASEN;
			}

			Peli::selvitys tmp_selvitys;

			tmp_selvitys.kone_id = valittu_kone;
			tmp_selvitys.nimi = komento;
			tmp_selvitys.toiminto = peli.toiminto;
			tmp_selvitys.aika = ohjelma.sekunnit() + apuvalineet::arvo_luku(3, 10);
			tmp_selvitys.kaarto = kaarto;

			peli.selvitykset.push_back(tmp_selvitys);
			peli.lisaa_selvityksia();
			ohjelma.odota(150);

			//lukija.tyhjenna();
			ohjelma.tyhjenna_viesti();
			peli.toiminto = Peli::TYHJA;
			peli.virheteksti = " ";
		}

		if (peli.kasitellyt >= asetukset.anna_asetus("vaadittavat_kasitellyt")) {
			peli.koska_uusi_kone = -1;
			if (peli.koneet.size() == 0) {
				/* Peli loppui */
				loppu = true;
			}
		}

		if (peli.etsi_valittu_kone() < 0) {
			peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_VALITSE_KONE);
		}
		else {
			peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_PAINA_TOIMINTONAPPAINTA);
		}

		for (unsigned int k = 0; k < peli.selvitykset.size(); ++k) {
			if ((int)peli.selvitykset[k].aika == (int)ohjelma.sekunnit()) {
				// TODO: Fix this
				/*switch (peli.selvitykset[k].toiminto) {
                    case peli.koneet[peli.selvitykset[k].kone_id].SUUNTA:
                        double tmp_suunta = apuvalineet::luvuksi<double>(peli.selvitykset[k].nimi);
                        peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(tmp_suunta, peli.selvitykset[k].toiminto, peli.selvitykset[k].kaarto);
                        peli.selvitykset.erase(peli.selvitykset.begin() + k);
                        break;
                    case peli.koneet[peli.selvitykset[k].kone_id].KORKEUS:
                    case peli.koneet[peli.selvitykset[k].kone_id].NOPEUS:
                        double tmp_muutos = apuvalineet::luvuksi<double>(peli.selvitykset[k].nimi);
                        peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(tmp_suunta, peli.selvitykset[k].toiminto);
                        peli.selvitykset.erase(peli.selvitykset.begin() + k);
                    case
				}*/
            }
		}

		peli.tarkista_porrastus();

		while (pelin_kello + peli.ajan_muutos <= ohjelma.sekunnit()) {
			pelin_kello += peli.ajan_muutos;
			peli.hoida_koneet();
		}
		view.piirra();
		ohjelma.odota();
	}

	// TODO: Tarvitaanko t‰t‰?
	ohjelma.tyhjenna_syote();

	logita_peliajat();

	return 0;
}

void PeliController::logita_peliajat() {
	double ka_alueella = 0;
	double ka_selvitykset = 0;
	double ka_alueelle = 0;

	for (unsigned int i = 0; i < peli.ajat.size(); ++i) {
		double alueella = peli.ajat[i].pois - peli.ajat[i].sisaan;
		ka_alueella += alueella;
		ka_selvitykset += peli.ajat[i].selvitykset;
		if (i > 0) {
			ka_alueelle += peli.ajat[i - 1].sisaan;
		}

		std::clog << peli.ajat[i].tunnus << " " << peli.ajat[i].sisaan << " " << peli.ajat[i].pois << " " << alueella << " " << peli.ajat[i].selvitykset << std::endl;
	}

	std::clog << peli.ajat.size() << " " << (ka_alueelle / peli.ajat.size()) << " " << ka_alueella << " " << ka_selvitykset << std::endl;
}

void PeliController::pyyda_atis() {
	AtisView atisView(view.piirtopinta, kieli, peli);
	AtisController atiscontroller(asetukset, kieli, ohjelma, atisView, peli);
	atiscontroller.aja();
}
