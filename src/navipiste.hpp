#ifndef _NAVIPISTE_HPP
#define _NAVIPISTE_HPP

#include "apuvalineet.hpp"
#include <string>

class navipiste {
	public:
		navipiste(std::string nimi, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta);
		navipiste();
		std::string nimi;
		apuvalineet::piste paikka;
		double lentosuunta;
		double lentokorkeus;
		double lentonopeus;

		bool operator ==(std::string n) {
			return (nimi == n);
		}
	};

#endif
