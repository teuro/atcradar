#include "pelicontroller.hpp"

bool PeliController::kasittele_komento(const std::string& komento) {
    /* Suunta nopeus korkeus */
    std::vector <std::string> asiat = apuvalineet::pilko_rivi(komento, "|");

    if (peli.valittuKone) {
        for (std::vector <std::string> :: iterator it = asiat.begin(); it < asiat.end(); ++it) {
            std::string selvitys = *it;
            ++it;
            int tyyppi = apuvalineet::luvuksi<int>(*it);

            anna_selvitys(selvitys, tyyppi);
        }
    }
}

bool PeliController::kasittele_aikaa(double intervallisek) {
    peli.hoida_koneet(intervallisek);    

    if (this->pelin_kello >= koska_uusi_kone) {
        peli.luo_kone(this->pelin_kello);
        koska_uusi_kone += apuvalineet::arvo_luku(asetukset.anna_asetus("koska_uusi_ala"), asetukset.anna_asetus("koska_uusi_yla"));
    }

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

}

void PeliController::anna_selvitys(std::string komento, int toiminto) {
    std::clog << komento << " " << toiminto << std::endl;
    if (komento == "ILS") {
        peli.valittuKone->ota_selvitys(apuvalineet::LAHESTYMIS, true);
        std::clog << "Annetaan lähestymisselvitys" << std::endl;
    } else if (komento == "CNL") {
        peli.valittuKone->ota_selvitys(apuvalineet::LAHESTYMIS, false);
        std::clog << "Annetaan oikotie" << std::endl;
    } else if (komento == "DCT") {
        peli.valittuKone->ota_selvitys(apuvalineet::OIKOTIE);
        std::clog << "Annetaan oikotie" << std::endl;
    } else if (komento == "HOLD") {
        std::clog << "Annetaan odotuskuvio" << std::endl;
		toiminto = apuvalineet::ODOTUS;
    } else if (komento == "OFF") {
        std::clog << "Annetaan odotuskuvion poisto" << std::endl;
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

	switch (toiminto) {
		case apuvalineet::KOHDE:
            peli.valittuKone->aseta_navipiste(*kohde);
			break;
        case apuvalineet::NOPEUS:
            std::clog << "Aseta nopeudeksi " << komento << std::endl;
            peli.valittuKone->muuta_selvitysnopeutta(apuvalineet::luvuksi<double>(komento));
            break;
        case apuvalineet::KORKEUS:
            std::clog << "Aseta korkeudeksi " << komento << std::endl;
            peli.valittuKone->muuta_selvityskorkeutta(apuvalineet::luvuksi<double>(komento));
            break;
        case apuvalineet::SUUNTA:
            std::clog << "Aseta suunnaksi " << komento << std::endl;
            peli.valittuKone->muuta_selvityssuuntaa(apuvalineet::luvuksi<double>(komento), kaarto);
            break;
	}
}

void PeliController::kasittele_hiiren_nappi(apuvalineet::piste koordinaatit) {
    peli.valitse_kone(koordinaatit);
}

void PeliController::kasittele_hiiren_paikka(apuvalineet::piste koordinaatit) {
    peli.aseta_hiiren_paikka(koordinaatit);
}
