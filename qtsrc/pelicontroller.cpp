#include "pelicontroller.hpp"

void PeliController::kasittele_komento(const std::string& komento) {
    std::vector <std::string> asiat = apuvalineet::pilko_rivi(komento, "|");
    std::vector <Peli::tilasto> :: iterator selvitykset = std::find(peli.ajat.begin(), peli.ajat.end(), peli.valittuKone->anna_kutsutunnus());

    if (peli.valittuKone) {
        for (std::vector <std::string> :: iterator it = asiat.begin(); it < asiat.end(); ++it) {
            std::string selvitys = *it;
            ++it;
            int tyyppi = apuvalineet::luvuksi<int>(*it);

            if (tarkista_selvitys(selvitys, tyyppi)) {
                ++selvitykset->selvitykset;
                anna_selvitys(selvitys, tyyppi);
            }
        }
    }
}

bool PeliController::tarkista_selvitys(std::string selvitys, int tyyppi) {
    std::vector <kiitotie> :: iterator haku_lasku = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), atis.anna_laskukiitotie());

    switch (tyyppi) {
    case apuvalineet::SUUNTA:
        if (peli.valittuKone->anna_selvityssuunta() == apuvalineet::luvuksi<int>(selvitys)) {
            return false;
        }
        break;
    case apuvalineet::NOPEUS:
        if (apuvalineet::luvuksi<int>(selvitys) < asetukset.anna_asetus("selvitysnopeus_ala")) {
            peli.aseta_virhe(Peli::VIRHE_NOPEUS_ALA);
            return false;
        } else if (apuvalineet::luvuksi<int>(selvitys) > asetukset.anna_asetus("selvitysnopeus_yla")) {
            peli.aseta_virhe(Peli::VIRHE_NOPEUS_YLA);
            return false;
        } else if (peli.valittuKone->anna_selvitysnopeus() == apuvalineet::luvuksi<int>(selvitys)) {
            return false;
        }
        break;
    case apuvalineet::KORKEUS:
        if (apuvalineet::luvuksi<int>(selvitys) < asetukset.anna_asetus("selvityskorkeus_ala")) {
            peli.aseta_virhe(Peli::VIRHE_KORKEUS_ALA);
            return false;
        } else if (apuvalineet::luvuksi<int>(selvitys) > asetukset.anna_asetus("selvityskorkeus_yla")) {
            peli.aseta_virhe(Peli::VIRHE_KORKEUS_YLA);
            return false;
        } else if (peli.valittuKone->anna_selvityskorkeus() == apuvalineet::luvuksi<int>(selvitys)) {
            return false;
        }
        break;
    case apuvalineet::LAHESTYMIS:
        if (peli.valittuKone->anna_nopeus() > asetukset.anna_asetus("maks_lahestymisnopeus")) {
            peli.aseta_virhe(Peli::VIRHE_LAHESTYMISNOPEUS);
            return false;
        }

        if (peli.valittuKone->anna_korkeus() > asetukset.anna_asetus("maks_lahestymiskorkeus")) {
            peli.aseta_virhe(Peli::VIRHE_LAHESTYMISKORKEUS);
            return false;
        }

        if (std::abs(peli.valittuKone->anna_suunta() - haku_lasku->suunta) > asetukset.anna_asetus("lahestymiskulma")) {
            peli.aseta_virhe(Peli::VIRHE_LAHESTYMISSUUNTA);
            return false;
        }
        break;
    case apuvalineet::OIKOTIE:
        if (peli.valittuKone->anna_korkeus() < asetukset.anna_asetus("oikotie")) {
            peli.aseta_virhe(Peli::VIRHE_OIKOTIE);
            return false;
        }
        break;
    default:
        break;
    }

    return true;
}

bool PeliController::kasittele_aikaa(double intervallisek) {
    peli.hoida_koneet(intervallisek);

    if (peli.anna_pelin_kello() >= peli.koska_uusi_kone && peli.koska_uusi_kone > 0) {
        if (peli.koneet.size() < asetukset.anna_asetus("maks_konemaara")) {
            peli.luo_kone();
        }

        if (peli.kasitellyt < asetukset.anna_asetus("vaadittavat_kasitellyt")) {
            int koska = apuvalineet::arvo_luku(asetukset.anna_asetus("koska_uusi_ala") / peli.anna_taso(), asetukset.anna_asetus("koska_uusi_yla") / peli.anna_taso());
            peli.koska_uusi_kone += koska;
        } else {
            peli.koska_uusi_kone = -1;
        }
    }

    this->ota_aika(intervallisek);

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

void PeliController::anna_selvitys(std::string komento, int toiminto) {    
    if (komento == "ILS") {
        peli.valittuKone->ota_selvitys(apuvalineet::LAHESTYMIS, true);
    } else if (komento == "CNL") {
        peli.valittuKone->ota_selvitys(apuvalineet::KESKEYTA, false);
    } else if (komento == "DCT") {
        peli.valittuKone->ota_selvitys(apuvalineet::OIKOTIE);
    } else if (komento == "HOLD") {
		toiminto = apuvalineet::ODOTUS;
    } else if (komento == "OFF") {
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
            peli.valittuKone->muuta_selvitysnopeutta(apuvalineet::luvuksi<double>(komento));
            break;
        case apuvalineet::KORKEUS:
            peli.valittuKone->muuta_selvityskorkeutta(apuvalineet::luvuksi<double>(komento));
            break;
        case apuvalineet::SUUNTA:
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
