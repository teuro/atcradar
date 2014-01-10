#ifndef _LENTOKONE_HPP
#define _LENTOKONE_HPP

#include "apuvalineet.hpp"
#include "navipiste.hpp"
#include "lentokentta.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>

class lentokone {
private:
        std::string kutsutunnus;

	double suunta;
	double korkeus;
	double nopeus;

	double nopeus_muutos;
	double korkeus_muutos;
	double suunta_muutos;

	double selvityssuunta;
	double selvityskorkeus;
	double selvitysnopeus;

	double odotuskuvio;

	int kaarron_suunta(double suunta);

	void muuta_korkeutta(double aika);
	void muuta_nopeutta(double aika);
	void muuta_suuntaa(double aika);

	void poista_reitti();

	kiitotie baana;
	lentokentta kentta;

	int kaarto;

	void tarkista_suunta_kohteeseen();

	void muuta_selvityskorkeutta(double korkeus);
	void muuta_selvitysnopeutta(double nopeus);
	void muuta_selvityssuuntaa(double suunta, int kaarto = apuvalineet::VASEN);

    void muuta_tilaa(double aika);

	bool oikotie;
    bool odotus;

    enum virheet {VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA};

	navipiste anna_piste();

	std::queue <navipiste> reitti;

	navipiste kohde;
	navipiste ulosmenopiste;

	void aseta_navipiste(navipiste paikka);
	void aseta_navipiste(apuvalineet::piste paikka);

	void lahesty();

	bool poistetaan;
	bool laskuselvitys;
public:
	lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);

    apuvalineet::piste paikka;

    void ota_selvitys(double suunta, int toiminto, int kaarto);
	void ota_selvitys(double komento, int toiminto);
	void ota_selvitys(navipiste& kohde);
	void ota_selvitys(int toiminto, kiitotie& baana, lentokentta& kentta);
	void ota_selvitys(int toiminto);

	void liiku(double aika);

	int tyyppi;

	bool lahestymisselvitys;

	bool onko_porrastus;

	bool operator ==(const lentokone& a) {
		return a.kutsutunnus == kutsutunnus;
	}

    lentokone& operator =(const lentokone&) { return *this; }

    double anna_nopeus() { return this->nopeus; }
    double anna_suunta() { return this->suunta; }
    double anna_korkeus() { return this->korkeus; }

    double anna_selvitysnopeus() { return this->selvitysnopeus; }
    double anna_selvityssuunta() { return this->selvityssuunta; }
    double anna_selvityskorkeus() { return this->selvityskorkeus; }

    std::string anna_kutsutunnus() { return this->kutsutunnus; }

    apuvalineet::piste anna_paikka() { return this->paikka; }

    void aseta_ulosmenopiste(navipiste kohde) { this->ulosmenopiste = kohde; }
    navipiste anna_ulosmenopiste() { return this->ulosmenopiste; }

    void aseta_odotus(bool venttaa) {  this->odotus = venttaa; }
    bool anna_odotus() { return this->odotus; }

    void aseta_poistetaan(bool poistuu) { this->poistetaan = poistuu; }
    bool anna_poistetaan() { return this->poistetaan; }

    void aseta_odotuskuvio(double aika) { this->odotuskuvio += aika; }
    double anna_odotuskuvio() { return this->odotuskuvio; }

    int anna_reitin_koko() { return this->reitti.size(); }

    bool anna_oikotie() { return this->oikotie; }
    bool anna_tyyppi() { return this->tyyppi; }

    navipiste anna_kohde() { return this->kohde; }
    bool anna_lahestymisselvitys() { return this->lahestymisselvitys; }
};

#endif
