#ifndef _KIELI_HPP
#define _KIELI_HPP

#include "apuvalineet.hpp"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Kieli {
private:
	std::string kieli;
	std::map <int, std::string> tekstit;
public:
	Kieli(const std::string& kieli);
	//Kieli(){ }
	void lisaa_teksti(int id, std::string teksti);
	std::string anna_teksti(int id);
	enum tekstit {
		TEKSTI_VALITSE_TASO = 1,
		TEKSTI_PORRASTUSVIRHEET,
		TEKSTI_UUSI_KONE_TULEE,
		TEKSTI_SEKUNNIT,
		TEKSTI_OHJE_VALITSE_KONE,
		TEKSTI_OHJE_PAINA_TOIMINTONAPPAINTA,
		TEKSTI_ONKO_ATIS_OK,
		TEKSTI_OHJE_SIIRTOPINTA_VAARIN,
		TEKSTI_OHJE_LAHTOBAANA_VAARIN,
		TEKSTI_OHJE_LASKUBAANA_VAARIN,
		TEKSTI_OHJE_ANNA_LAHTOKIITOTIE,
		TEKSTI_OHJE_ANNA_LASKUKIITOTIE,
		TEKSTI_OHJE_ANNA_SIIRTOPINTA,
		TEKSTI_OHJE_LAHTOKIITOTIE,
		TEKSTI_OHJE_LASKUKIITOTIE,
		TEKSTI_OHJE_SIIRTOPINTA,
		TEKSTI_OHJE_SUUNTA,
		TEKSTI_OHJE_NOPEUS,
		TEKSTI_OHJE_KORKEUS,
		TEKSTI_KUTSUTUNNUS,
		TEKSTI_ALUEELLE,
		TEKSTI_POIS,
		TEKSTI_ALUEELLA,
        TEKSTI_SELVITYKSET,
        TEKSTI_TIEDOSTO,
        TEKSTI_EI_AUKEA
	};
};

#endif
