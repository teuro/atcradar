// peli.hpp
#ifndef _PELI_HPP
#define _PELI_HPP

#include <vector>
#include <stdexcept>
#include "apuvalineet.hpp"
#include "lentokone.hpp"
#include "lentokentta.hpp"
#include "navipiste.hpp"

namespace peli {
	extern std::vector <lentokone> koneet;
	extern std::vector <lentokone> odottavat;
	extern const float ajan_muutos;
	extern int kasitellyt;
	extern std::string ohje;
	extern std::string virheteksti;
	extern int porrastusvirheet;
	extern int muut_virheet;
	enum virheet {VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA};
	enum lukeminen {SUUNTA=1, NOPEUS, KORKEUS, TYHJENNA, LAHESTYMIS, OIKOTIE, ODOTUS, POIS, TYHJA = 0};
	enum tyyppi {SAAPUVA = 0, LAHTEVA = 1};
	enum atis_toiminnot {LAHTO, LASKU, SIIRTOPINTA};
	extern int toiminto;
	void luo_kone();
	void tuhoa_kone(int kone);

	struct tilasto {
		std::string tunnus;
		double sisaan;
		double pois;
		int selvitykset;

		bool operator ==(std::string kutsutunnus) {
			return kutsutunnus == tunnus;
		}
	};

	namespace atis {
		extern int lahtokiitotie;
		extern int laskukiitotie;
		extern std::string lahto;
		extern std::string lasku;
		extern double siirtopinta;
		extern int siirtokorkeus;
		extern bool ok;
	}

	namespace metar {
		extern int tuuli;
		extern int voimakkuus;
		extern int puuskat;
		extern double paine;
		extern int nakyvyys;
		extern int lampotila;
		extern int kastepiste;
	}

	extern std::vector <navipiste> navipisteet;
	extern std::vector <navipiste> sisapisteet;
	extern lentokentta kentta;

	int aja();
	void aseta_virhe(int virhe);
	extern std::vector <tilasto> ajat;
	extern std::string syote;
}

#endif
