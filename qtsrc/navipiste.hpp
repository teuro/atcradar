#ifndef _NAVIPISTE_HPP
#define _NAVIPISTE_HPP

#include "apuvalineet.hpp"
#include <string>
#include <iostream>

class navipiste {
	public:
        navipiste(std::string nimi, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi);
		navipiste(std::string nimi, apuvalineet::piste paikka);
        navipiste() {}

		std::string nimi;

        apuvalineet::piste paikka;

		double lentosuunta;
		double lentokorkeus;
		double lentonopeus;
        int tyyppi;

		bool operator ==(std::string n) {
			return (nimi == n);
		}

        bool operator ==(int tyyppi) {
            return (this->tyyppi == tyyppi);
        }
	};

#endif
