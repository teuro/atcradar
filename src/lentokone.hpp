#ifndef _LENTOKONE_HPP
#define _LENTOKONE_HPP

#include "apuvalineet.hpp"
#include "navipiste.hpp"
#include "lentokentta.hpp"

#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>

const int VASEN = -1;
const int OIKEA = 1;

class lentokone {
private:
	int kaarron_suunta(double suunta);
	void muuta_korkeutta(double aika);
	void muuta_nopeutta(double aika);
	void muuta_suuntaa(double aika);
	double nopeus_muutos;
	double korkeus_muutos;
	double suunta_muutos;
public:
	navipiste anna_piste();
	std::queue <navipiste> reitti;
	apuvalineet::piste paikka;
	navipiste kohde;
	navipiste ulosmenopiste;

	bool odotus;
	bool oikotie;
	lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);

//    lentokone(const lentokone& lk);

	void ota_selvitys(double suunta, int toiminto, int kaarto);
	void ota_selvitys(double komento, int toiminto);
	void ota_selvitys(navipiste& kohde);
	void ota_selvitys(int toiminto);

	void muuta_selvityksia(double korkeus, double nopeus, double suunta, int kaarto = VASEN);
	void muuta_tilaa(double aika);

	void muuta_selvityskorkeutta(double korkeus);
	void muuta_selvitysnopeutta(double nopeus);
	void muuta_selvityssuuntaa(double suunta, int kaarto = VASEN);

	void liiku(double aika);

	std::string kutsutunnus;
	double suunta;
	double korkeus;
	double nopeus;
	bool valittu;
	int tyyppi;
	bool poistetaan;

	double odotuskuvio;
	bool lahestymisselvitys;
	bool laskuselvitys;
	bool ylosveto;

	double selvityssuunta;
	double selvityskorkeus;
	double selvitysnopeus;

	void tarkista_suunta_kohteeseen();

	void aseta_navipiste(navipiste paikka);
	void aseta_navipiste(apuvalineet::piste paikka);

	kiitotie baana;
	double reaktioaika;
	double polttoaine;
	void lahesty();

	int kaarto;
	bool onko_porrastus;

	bool operator ==(const lentokone& a) {
		return a.kutsutunnus == kutsutunnus;
	}

    lentokone& operator =(const lentokone&) { return *this; }

	void poista_reitti();
private:
    enum virheet {VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA};
	enum lukeminen {SUUNTA=1, NOPEUS, KORKEUS, TYHJENNA, LAHESTYMIS, OIKOTIE, ODOTUS, POIS, TYHJA = 0};
};

#endif
