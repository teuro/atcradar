// peli.hpp
#ifndef _PELI_HPP
#define _PELI_HPP

#include <vector>
#include <stdexcept>
#include "apuvalineet.hpp"
#include "lentokone.hpp"
#include "lentokentta.hpp"
#include "navipiste.hpp"

class PeliView;
class Peli;
class Ohjelma;

class PeliController {
    Peli& peli;
    PeliView& view;
    Ohjelma& ohjelma;
public:
    PeliController(Peli& p, PeliView& v, Ohjelma& o) : peli(p), view(v), ohjelma(o) { }
    int aja();
    void pyyda_atis();
};

class Peli {
public:
    Peli(Ohjelma &o) : ohjelma(o) 
    {

    }

    Ohjelma& ohjelma;

    const float ajan_muutos = 0.02;
    std::vector <std::string> tunnukset;
    std::string syote;

    void lataa_kentta(std::string kentta);
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
    std::vector <lentokone> odottavat;

    std::vector <navipiste> navipisteet;
    std::vector <navipiste> sisapisteet;

    std::string generoi_tunnus();

    void valitse_kone(const apuvalineet::piste& hiiri);

    void tarkista_porrastus();
    void lataa_navipisteet(std::string nimi);
    void lataa_lahipisteet(std::string nimi);

    bool onko_vapaata();
    void hoida_koneet();
    void poista_kone(int kone);
    int etsi_valittu_kone();

    //void pyyda_atis();
    bool tarkista_atis();
    void generoi_metar();

    lentokentta kentta;

    int alku;
    int toiminto;
    int koska_uusi_kone = 1;
    int koska_metar;
    int porrastusvirheet = 0;
    int muut_virheet = 0;

    struct Atis {
        int lahtokiitotie = -1;
        int laskukiitotie = -1;
        std::string lahto;
        std::string lasku;
        double siirtopinta = -1;
        int siirtokorkeus;
        bool ok = false;

        struct Paine {
            double alaraja;
            double ylaraja;
            double siirtopinta;
        } paine;

        std::vector <Paine> paineet;
        void lue_paineet(std::string nimi);
        double etsi_siirtopinta(double paine);
    } atis;

   
	enum virheet {VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA};
	enum lukeminen {SUUNTA=1, NOPEUS, KORKEUS, TYHJENNA, LAHESTYMIS, OIKOTIE, ODOTUS, POIS, TYHJA = 0};
	enum tyyppi {SAAPUVA = 0, LAHTEVA = 1};
	enum atis_toiminnot {LAHTO, LASKU, SIIRTOPINTA};
	void luo_kone(Ohjelma& ohjelma);
	void tuhoa_kone(int kone);

	/*struct Atis {
		int lahtokiitotie;
		int laskukiitotie;
		std::string lahto;
		std::string lasku;
		double siirtopinta;
		int siirtokorkeus;
		bool ok;
    } atis;*/

	struct Metar {
		int tuuli;
		int voimakkuus;
		int puuskat;
		double paine;
		int nakyvyys;
		int lampotila;
		int kastepiste;
		int ilmankosteus;
    } metar;

	void aseta_virhe(int virhe);
};

#endif
