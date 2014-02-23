#ifndef _PELI_CONTROLLER_HPP
#define _PELI_CONTROLLER_HPP

#include <QObject>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <list>

#include "controller.hpp"
#include "asetukset.hpp"
#include "peli.hpp"
#include "AtisController.hpp"

/*
    MVC Ohjain

    PeliController lukee ja tulkitsee syötteet. Käsitellyt syötteiden perusteella lasketaan pelille uusi tila.
    Ohjain piirtää pelin tilan käyttäen peliview-luokkaa.
*/

class PeliController : public IController {
	Peli& peli;
	IAsetukset& asetukset;
    IPiirtoPinta& piirtopinta;
    Atis& atis;
public:
    PeliController(Peli& p, IAsetukset& a, IPiirtoPinta& pinta, Atis& at) : peli(p), asetukset(a), piirtopinta(pinta), atis(at)  { }
	
	bool kasittele_aikaa(double sekunnit);
	void kasittele_hiiren_nappi(apuvalineet::piste koordinaatit);
	void kasittele_hiiren_paikka(apuvalineet::piste koordinaatit);

    void kasittele_komento(const std::string& komento);

    void anna_selvitys(std::string komento, int toiminto);

    void ota_aika(double aika) {
        peli.aseta_pelin_kello(aika);
    }

private:
    bool tarkista_selvitys(std::string selvitys, int tyyppi);
	void logita_peliajat();
};

#endif
