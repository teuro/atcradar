#ifndef _LENTOKONE_HPP
#define _LENTOKONE_HPP

#include "apuvalineet.hpp"

const int VASEN = -1;
const int OIKEA = 1;

class lentokone {
private:
	int kaarron_suunta(double suunta);
public:
	apuvalineet::piste paikka;
	apuvalineet::piste kohde;
	bool odotus;
	lentokone(std::string kutsutunnus, double x, double y, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);
	lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus);

	lentokone();

	void ota_selvitys(std::string tmp, int toiminto);

	void muuta_tilaa(double aika);

	void muuta_selvityskorkeutta(double korkeus);
	void muuta_selvitysnopeutta(double nopeus);
	void muuta_selvityssuuntaa(double suunta, int kaarto = VASEN);

	void liiku(double aika);

	std::string kutsutunnus;
	int ulosmenopiste;
	double suunta;
	double korkeus;
	double nopeus;
	bool valittu;
	int tyyppi;

	bool lahestymisselvitys;
	bool laskuselvitys;
	bool ylosveto;

	double selvityssuunta;
	double selvityskorkeus;
	double selvitysnopeus;

	void muuta_korkeutta(double aika);
	void muuta_nopeutta(double aika);
	void muuta_suuntaa(double aika);

	void tarkista_suunta_kohteeseen();
	bool mitataan;

	void aseta_navipiste(apuvalineet::piste paikka);

	int laskubaana;
	double polttoaine;
	void lahesty();

	int kaarto;
	bool onko_porrastus;

	bool operator ==(const lentokone& a) {
		return a.kutsutunnus == kutsutunnus;
	}
};

#endif
