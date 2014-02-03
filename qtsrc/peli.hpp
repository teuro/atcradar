#ifndef _PELI_HPP
#define _PELI_HPP

#include "apuvalineet.hpp"
#include "lentokone.hpp"
#include "lentokentta.hpp"
#include "navipiste.hpp"
#include "asetukset.hpp"
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
    MVC Malli

    Peliluokka tarjoaa matalan tason laskentaa pelin sisäisen tilan päivittämiseksi. Tätä luokkaa käyttävät
    kontrolleri ja näkymä.
*/

class Peli {
private:
    IAsetukset& asetukset;
    Kieli& kieli;
    Atis& atis;
    int taso;
    double pelin_kello;
    void logita_aika(lentokone *lk);
    double edellinen_kone_lahto;
public:
    Peli(IAsetukset& a, Kieli& k, Atis& at, Metar& m);

    int toiminto;
    int koska_uusi_kone;
    int koska_metar;
    int porrastusvirheet;
    int muut_virheet;
    int kasitellyt;

    struct tilasto {
        std::string tunnus;
        double sisaan;
        double pois;
        int selvitykset;
        double pisteet;

        bool operator ==(std::string kutsutunnus) {
            return kutsutunnus == tunnus;
        }
    };

    Metar& metar;
    lentokone* valittuKone;

    std::vector <std::string> tunnukset;
    std::vector <tilasto> ajat;
    std::vector <navipiste> navipisteet;
    std::vector <navipiste> sisapisteet;

    std::string syote;

    void lataa_tunnukset(std::string tunnukset);
    void lisaa_selvityksia();
    void valitse_kone(const apuvalineet::piste& hiiri);
    void aseta_hiiren_paikka(const apuvalineet::piste& hiiri);
    void tarkista_porrastus();
    void hoida_koneet(double intervalliMs);
    void generoi_metar();
    void luo_kone();
    void aseta_taso(int taso);
    void aseta_virhe(int virhe);
    void aseta_pelin_kello(double aika);
    void aloita();
    void lataa_kentta(std::string kentta);

    double anna_pelin_kello();

    std::string ohje;
    std::string virheteksti;

    std::list <lentokone*> koneet;
    std::queue <lentokone> odottavat;

    std::string generoi_tunnus();

    apuvalineet::piste hiiren_paikka;
    lentokentta kentta;

    bool onko_vapaata(int tyyppi = LAHTEVA, int piste = -1);

    int anna_taso();

    enum tyyppi {SAAPUVA = 0, LAHTEVA = 1};
    enum atis_toiminnot {LAHTO, LASKU, SIIRTOPINTA};
    enum virheet { VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_OIKOTIE, VIRHE_PORRASTUS, VIRHE_ALUEELTA };

    std::vector <std::string> lataa_pilvet(std::string pilvet);
};

#endif
