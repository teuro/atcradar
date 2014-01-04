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

void PeliController::kasittele_hiiren_nappi(apuvalineet::piste koordinaatit)
{
    peli.aseta_hiiren_paikka(koordinaatit);
	peli.valitse_kone(koordinaatit);
}

void PeliController::kasittele_hiiren_paikka(apuvalineet::piste koordinaatit)
{
    peli.aseta_hiiren_paikka(koordinaatit);
}

// Pelin pääfunktio. Käsittelee kaiken inputin ja syötteet ja välittää ne eri funktiolla itselleen.
// Muutetaan jatkossa niin, että kaikki input tulee ulkopuolelta eventetteinä em. funktiolle
// ja tämä voidaan poistaa.
int PeliController::aja() {
	std::clog << "pelicontroller::aja()" << std::endl;

	std::srand((unsigned int)std::time(NULL));
	ajastimet.push_back(ajastin("metar", asetukset.anna_asetus("koska_metar"), 0));

	pyyda_atis();
	peli.luo_kone();
	peli.luo_kone();

	double prev = ohjelma.sekunnit(true);
	bool jatka = true;
	while (jatka)
	{
		if (ohjelma.lue_hiiri())
		{
			apuvalineet::piste hiiri = ohjelma.anna_hiiri();
			if (ohjelma.lue_hiiri())
			{
				kasittele_hiiren_nappi(hiiri);
			}
			else
			{
				kasittele_hiiren_paikka(hiiri);
			}
		}

		double nyt = ohjelma.sekunnit();
		jatka = kasittele_aikaa(nyt - prev);
		prev = nyt;

		peli.syote = ohjelma.lue_syote();

		IOhjelma::nappi nappi = ohjelma.lue_nappi();

		if (nappi != Ohjelma::NAPPI_MUU)
		{
			if (nappi == IOhjelma::NAPPI_ENTER)
			{
				kasittele_komento(ohjelma.anna_viesti());
				ohjelma.tyhjenna_viesti();
			}
			else
			{
				if (!kasittele_nappi(nappi))
				{
					jatka = false;
				}
			}
		}

		view.piirra(piirtopinta);
		ohjelma.odota(10); // 10 ms
	}

	logita_peliajat();
	return 0;
}

bool PeliController::kasittele_nappi(IOhjelma::nappi nappi)
{
	switch (nappi)
	{
		case IOhjelma::NAPPI_ESCAPE:
			return false;
		case IOhjelma::NAPPI_F5:
			peli.toiminto = apuvalineet::SUUNTA;
			break;
		case IOhjelma::NAPPI_F7:
			peli.toiminto = apuvalineet::NOPEUS;
			break;
		case IOhjelma::NAPPI_F8:
			peli.toiminto = apuvalineet::KORKEUS;
			break;
        default:
            break;
	}
	return true;
}

bool PeliController::kasittele_komento(const std::string& komento)
{
	int valittu_kone = peli.etsi_valittu_kone();

	if (valittu_kone >= 0) {
		peli.selvitykset.push_back(anna_selvitys(komento, peli.toiminto));
		peli.selvitykset.back().kone_id = valittu_kone;
		peli.selvitykset.back().aika = pelin_kello + 2 * 1000.0;// ohjelma.sekunnit() + 2;

		peli.lisaa_selvityksia();
		peli.toiminto = apuvalineet::TYHJA;
		peli.virheteksti = " ";
	}
	return true;
}

// kasittele_aikaa(), jota kutsutaan jatkuvasti antaen parametrina
// kulunut aikaintervalli. Palauttaa false, jos peli päättyy.
bool PeliController::kasittele_aikaa(double intervallisek) {
	pelin_kello += intervallisek;
	if (peli.porrastusvirheet >= asetukset.anna_asetus("maks_porrastusvirhe")) {
		std::clog << kieli.anna_teksti(Kieli::TEKSTI_PORRASTUSVIRHEET) << std::endl;
		//loppu = true;
		return false;
	}

	if (pelin_kello >= peli.koska_uusi_kone) {
		if ((int)peli.koneet.size() < (int)asetukset.anna_asetus("maks_konemaara")) {
			peli.luo_kone();
			peli.koska_uusi_kone += apuvalineet::arvo_luku(asetukset.anna_asetus("koska_uusi_ala"), asetukset.anna_asetus("koska_uusi_yla"));
		}

		std::clog << kieli.anna_teksti(Kieli::TEKSTI_UUSI_KONE_TULEE) << " " << (peli.koska_uusi_kone - pelin_kello) << " " << kieli.anna_teksti(Kieli::TEKSTI_SEKUNNIT) << std::endl;
	}

	if (pelin_kello >= peli.koska_metar) {
		peli.generoi_metar();

		if (!peli.tarkista_atis()) {
			peli.atis.ok = false;
			pyyda_atis();
		}

		peli.koska_metar += asetukset.anna_asetus("koska_metar");
	}

	if (peli.kasitellyt >= asetukset.anna_asetus("vaadittavat_kasitellyt")) {
		peli.koska_uusi_kone = -1;
		if (peli.koneet.size() == 0) {
			/* Peli loppui */
			return false;
		}
	}

	if (peli.etsi_valittu_kone() < 0) {
		peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_VALITSE_KONE);
	}
	else {
		peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_PAINA_TOIMINTONAPPAINTA);
	}

	for (unsigned int k = 0; k < peli.selvitykset.size(); ++k) {
	//	std::clog << peli.selvitykset[k].kone_id << " " << (int)peli.selvitykset[k].aika << " " << (int)ohjelma.sekunnit() << " " << peli.selvitykset[k].kohde.nimi << std::endl;
		if ((int)peli.selvitykset[k].aika == (int)ohjelma.sekunnit()) {
			switch (peli.selvitykset[k].toiminto) {
				case apuvalineet::KOHDE:
					peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(peli.selvitykset[k].kohde);
					peli.selvitykset.erase(peli.selvitykset.begin()+k);
					break;
                case apuvalineet::SUUNTA:
                    peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(peli.selvitykset[k].suunta, peli.selvitykset[k].toiminto, peli.selvitykset[k].kaarto);
                    peli.selvitykset.erase(peli.selvitykset.begin()+k);
                    break;
                case apuvalineet::NOPEUS:
                    peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(peli.selvitykset[k].nopeus, peli.selvitykset[k].toiminto);
                    peli.selvitykset.erase(peli.selvitykset.begin()+k);
                    break;
                case apuvalineet::KORKEUS:
                    peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(peli.selvitykset[k].korkeus, peli.selvitykset[k].toiminto);
                    peli.selvitykset.erase(peli.selvitykset.begin()+k);
                    break;
                case apuvalineet::OIKOTIE:
                    peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(peli.selvitykset[k].toiminto);
                    break;
                case apuvalineet::LAHESTYMIS:
                    peli.koneet[peli.selvitykset[k].kone_id].ota_selvitys(peli.selvitykset[k].toiminto, peli.kentta.kiitotiet[peli.atis.laskukiitotie], peli.kentta);
                    break;
                default:
                    break;
			}
        }
	}

	peli.tarkista_porrastus();
	peli.hoida_koneet(intervallisek);

	return true;
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
	AtisView atisView(kieli, peli);
	AtisController atiscontroller(asetukset, kieli, ohjelma, atisView, peli, piirtopinta);
	atiscontroller.aja();
}

Peli::selvitys PeliController::anna_selvitys(std::string komento, int toiminto) {
	if ((komento == "ILS" || komento == "ils")) {
		toiminto = apuvalineet::LAHESTYMIS;
	}
	else if ((komento == "DCT" || komento == "dct")) {
		toiminto = apuvalineet::OIKOTIE;
	}
	else if (komento == "HOLD" || komento == "hold") {
		toiminto = apuvalineet::ODOTUS;
	}
	else if (komento == "OFF" || komento == "off") {
		toiminto = apuvalineet::POIS;
	}

	int kaarto;
	std::vector <navipiste> :: iterator kohde;

	if (komento.substr(0, 1) == "O") {
        kaarto = apuvalineet::OIKEA;
        komento = komento.substr(1);
	} else if (komento.substr(0, 1) == "V") {
        kaarto = apuvalineet::VASEN;
        komento = komento.substr(1);
	} else {
        kaarto = apuvalineet::VASEN;
	}

    if (komento.length() == 5 && toiminto == apuvalineet::SUUNTA) {
        kohde = std::find(peli.sisapisteet.begin(), peli.sisapisteet.end(), komento);

        if (kohde != peli.navipisteet.end()) {
            toiminto = apuvalineet::KOHDE;
        }
    }

	Peli::selvitys tmp_selvitys;

	switch (toiminto) {
		case apuvalineet::KOHDE:
			tmp_selvitys.kohde = *kohde;
			break;
        case apuvalineet::NOPEUS:
            tmp_selvitys.nopeus = apuvalineet::luvuksi<double>(komento);
            break;
        case apuvalineet::KORKEUS:
            tmp_selvitys.korkeus = apuvalineet::luvuksi<double>(komento);
            break;
        case apuvalineet::SUUNTA:
            tmp_selvitys.suunta = apuvalineet::luvuksi<double>(komento);
            tmp_selvitys.kaarto = kaarto;
            break;
	}

	tmp_selvitys.toiminto = toiminto;

	return tmp_selvitys;
}
