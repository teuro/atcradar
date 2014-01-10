#ifndef _PELI_HPP
#define _PELI_HPP

#include "apuvalineet.hpp"
#include "lentokone.hpp"
#include "lentokentta.hpp"
#include "navipiste.hpp"
#include "asetukset.h"
#include "kieli.hpp"
#include "view.hpp"
#include "AtisController.hpp"
#include "Metar.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <stdexcept>
#include <string>

/*
MVC Model

Peli class maintains the game's runtime state and provides helper methods to update the state. Nothing else.
See PeliController and PeliView for more functionality.
*/

/*
	* Peli ottaa vastuun pelin eri osa-alueista. Toimii yhteistyössä näkymän ja kontrollerin kanssa.
*/

class Peli {
private:
    IAsetukset& asetukset;
    Atis& atis;

public:
    Metar& metar;
    Peli(IAsetukset& a, Kieli& kieli, std::string kentta, Metar& m, Atis& at);

//    double ajan_muutos;
    std::vector <std::string> tunnukset;
    std::string syote;

    void lataa_tunnukset(std::string tunnukset);

    void lisaa_selvityksia();

    struct tilasto {
        std::string tunnus;
        double sisaan;
        double pois;
        int selvitykset;

        bool operator ==(std::string kutsutunnus) {
            return kutsutunnus == tunnus;
        }
    };
    std::vector <tilasto> ajat;

    std::string ohje;
    std::string virheteksti;

    int kasitellyt;

    std::list <lentokone*> koneet;
    lentokone* valittuKone;
    std::queue <lentokone> odottavat;

	struct selvitys {
		int kone_id;
		double suunta;
		double korkeus;
		double nopeus;
		navipiste kohde;
		int toiminto;
		double aika;
		int kaarto;
	};

	std::vector <selvitys> selvitykset;

    std::vector <navipiste> navipisteet;
    std::vector <navipiste> sisapisteet;

    std::string generoi_tunnus();

    void valitse_kone(const apuvalineet::piste& hiiri);
    void aseta_hiiren_paikka(const apuvalineet::piste& hiiri);
    apuvalineet::piste hiiren_paikka;

    void tarkista_porrastus();

    bool onko_vapaata(int tyyppi = LAHTEVA, int piste = -1);
    void hoida_koneet(double intervalliMs);

    bool tarkista_atis();
    void generoi_metar();

    lentokentta kentta;

    int toiminto;
    int koska_uusi_kone;
    int koska_metar;
    int porrastusvirheet;
    int muut_virheet;

	enum tyyppi {SAAPUVA = 0, LAHTEVA = 1};
	enum atis_toiminnot {LAHTO, LASKU, SIIRTOPINTA};
    void luo_kone(double aika);
	void tuhoa_kone(int kone);

	void aseta_virhe(int virhe);
	std::vector <std::string> lataa_pilvet(std::string pilvet);
	enum virheet { VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA };
private:
	void lataa_kentta(std::string kentta);
};

#endif
