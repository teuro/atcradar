#ifndef _PELI_HPP
#define _PELI_HPP

#include "apuvalineet.hpp"
#include "lentokone.hpp"
#include "lentokentta.hpp"
#include "navipiste.hpp"
#include "asetukset.h"
#include "kieli.hpp"
#include "ohjelma.hpp"
#include "lukija.hpp"
#include "ajastin.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
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

public:
	Peli(IAsetukset& a, IOhjelma &o, Kieli& kieli, std::string kentta);

    IOhjelma& ohjelma;

    float ajan_muutos;
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

    std::vector <lentokone> koneet;
    std::queue <lentokone> odottavat;

	struct selvitys {
		int kone_id;
		std::string nimi;
		int toiminto;
		float aika;
		int kaarto;
	};

	std::vector <selvitys> selvitykset;

    std::vector <navipiste> navipisteet;
    std::vector <navipiste> sisapisteet;

    std::string generoi_tunnus();

    void valitse_kone(const apuvalineet::piste& hiiri);

    void tarkista_porrastus();

    bool onko_vapaata(int tyyppi = LAHTEVA, int piste = -1);
    void hoida_koneet();
    int etsi_valittu_kone();

    bool tarkista_atis();
    void generoi_metar();

    lentokentta kentta;

    int toiminto;
    int koska_uusi_kone;
    int koska_metar;
    int porrastusvirheet;
    int muut_virheet;

    struct Atis {
		std::string ohje;
		int lahtokiitotie;// = -1;
		int laskukiitotie;// = -1;
		double siirtopinta;// = -1;
		std::string lahto;
        std::string lasku;
        int siirtokorkeus;
        bool ok;

        struct Paine {
            double alaraja;
            double ylaraja;
            double siirtopinta;
        } paine;

        std::vector <Paine> paineet;
        void lue_paineet(std::string nimi);
        double etsi_siirtopinta(double paine);
    } atis;


	enum tyyppi {SAAPUVA = 0, LAHTEVA = 1};
	enum atis_toiminnot {LAHTO, LASKU, SIIRTOPINTA};
	void luo_kone();
	void tuhoa_kone(int kone);

	struct Metar {
		int tuuli;
		int voimakkuus;
		int puuskat;
		double paine;
		int nakyvyys;
		int lampotila;
		int kastepiste;
		int ilmankosteus;
		std::map <std::string, int> pilvet;
    } metar;

	void aseta_virhe(int virhe);
	std::vector <std::string> lataa_pilvet(std::string pilvet);
	enum lukeminen { SUUNTA = 1, NOPEUS, KORKEUS, TYHJENNA, LAHESTYMIS, OIKOTIE, ODOTUS, POIS, TYHJA = 0 };
	enum virheet { VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA };
private:
		void lataa_kentta(std::string kentta);
};

#endif
