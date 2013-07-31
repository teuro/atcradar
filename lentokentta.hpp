#ifndef _LENTOKENTTA_HPP
#define _LENTOKENTTA_HPP

#include "apuvalineet.hpp"
#include <vector>

class kiitotie {
public:
	apuvalineet::piste alkupiste;
	apuvalineet::piste loppupiste;
	apuvalineet::piste hidastuspiste;
	apuvalineet::piste lahestymispiste;
	apuvalineet::piste odotuspiste;
	std::string nimi;

	double suunta;
	double pituus;
	double alkunousukorkeus;
	double alkunoususuunta;
	bool valittu;

	kiitotie(std::string nimi, apuvalineet::piste alkupiste, double pituus, double suunta, double alkunousukorkeus, double alkunousuuunta);

	apuvalineet::piste laske_loppupiste();
	apuvalineet::piste laske_lahestymispiste();
	apuvalineet::piste laske_hidastuspiste();
	apuvalineet::piste laske_odotuspiste();

	bool operator==(std::string n) {
		return n == nimi;
	}
};

class lentokentta {
public:
	std::vector <kiitotie> kiitotiet;
	double korkeus;
	apuvalineet::piste paikka;
	std::string nimi;
};



#endif
