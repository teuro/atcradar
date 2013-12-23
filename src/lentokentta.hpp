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

	kiitotie(std::string nimi, apuvalineet::piste alkupiste, double pituus, double suunta, double alkunousukorkeus, double alkunousuuunta, double lahestymispiste, double hidastuspiste);

	bool operator ==(std::string n) {
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
