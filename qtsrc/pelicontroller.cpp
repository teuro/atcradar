#include "pelicontroller.hpp"

bool PeliController::kasittele_nappi(IOhjelma::nappi nappi) {
    switch (nappi) {
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

bool PeliController::kasittele_komento(const std::string& komento) {
	return true;
}

// kasittele_aikaa(), jota kutsutaan jatkuvasti antaen parametrina
// kulunut aikaintervalli. Palauttaa false, jos peli p‰‰ttyy.
bool PeliController::kasittele_aikaa(double intervallisek) {

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

void PeliController::kasittele_hiiren_nappi(apuvalineet::piste koordinaatit) {

}

void PeliController::kasittele_hiiren_paikka(apuvalineet::piste koordinaatit) {

}
