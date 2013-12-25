#ifndef _LENTOKONE_HPP
#define _LENTOKONE_HPP

#include "apuvalineet.hpp"
#include "navipiste.hpp"
#include "asetukset.h"
#include <vector>
#include <queue>
#include <fstream>

const int VASEN = -1;
const int OIKEA = 1;

class Peli;
class IOhjelma;

class lentokone {
private:
	int kaarron_suunta(double suunta);
	void muuta_korkeutta(double aika);
	void muuta_nopeutta(double aika);
	void muuta_suuntaa(double aika);

	// TODO: N‰ist‰ riippuvuuksista olisi hyv‰ p‰‰st‰ eroon.
	Peli& peli;
    IOhjelma& ohjelma;
	IAsetukset& asetukset;

public:
	navipiste anna_piste();
	std::queue <navipiste> reitti;
	apuvalineet::piste paikka;
	navipiste kohde;
	navipiste ulosmenopiste;

	bool odotus;
	bool oikotie;
	lentokone(Peli& p, IOhjelma& o, IAsetukset& a, std::string kutsutunnus, double x, double y, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);
	lentokone(Peli& p, IOhjelma& o, IAsetukset& a, std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);

//    lentokone(const lentokone& lk);

	void ota_selvitys(std::string tmp, int toiminto, int kaarto = 0);

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

	int laskubaana;
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

};

#endif
