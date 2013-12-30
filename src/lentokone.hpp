#ifndef _LENTOKONE_HPP
#define _LENTOKONE_HPP

#include "apuvalineet.hpp"
#include "navipiste.hpp"
#include "lentokentta.hpp"

#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>

class lentokone {
public:
	std::string kutsutunnus;
	double suunta;
	double korkeus;
	double nopeus;
	int kaarron_suunta(double suunta);
	void muuta_korkeutta(double aika);
	void muuta_nopeutta(double aika);
	void muuta_suuntaa(double aika);
	double nopeus_muutos;
	double korkeus_muutos;
	double suunta_muutos;
	void poista_reitti();
    double selvityssuunta;
	double selvityskorkeus;
	double selvitysnopeus;
	kiitotie baana;
	int kaarto;
	void tarkista_suunta_kohteeseen();
	void muuta_selvityskorkeutta(double korkeus);
	void muuta_selvitysnopeutta(double nopeus);
	void muuta_selvityssuuntaa(double suunta, int kaarto = apuvalineet::VASEN);
    void muuta_tilaa(double aika);
	bool oikotie;
    enum virheet {VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA};
	bool odotus;
	navipiste anna_piste();
	std::queue <navipiste> reitti;
	apuvalineet::piste paikka;
	navipiste kohde;
	navipiste ulosmenopiste;

	lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);

	void ota_selvitys(double suunta, int toiminto, int kaarto);
	void ota_selvitys(double komento, int toiminto);
	void ota_selvitys(navipiste& kohde);
	void ota_selvitys(int toiminto, kiitotie& baana);
	void ota_selvitys(int toiminto);

	void liiku(double aika);

	bool valittu;
	int tyyppi;
	bool poistetaan;

	double odotuskuvio;
	bool lahestymisselvitys;
	bool laskuselvitys;
	bool ylosveto;

	void aseta_navipiste(navipiste paikka);
	void aseta_navipiste(apuvalineet::piste paikka);

	void lahesty();

	bool onko_porrastus;

	bool operator ==(const lentokone& a) {
		return a.kutsutunnus == kutsutunnus;
	}

    lentokone& operator =(const lentokone&) { return *this; }
    //enum lukeminen {SUUNTA=1, NOPEUS, KORKEUS, TYHJENNA, LAHESTYMIS, OIKOTIE, ODOTUS, POIS, TYHJA = 0};

    double anna_nopeus() { return this->nopeus; }
    double anna_suunta() { return this->suunta; }
    double anna_korkeus() { return this->korkeus; }
};

#endif
