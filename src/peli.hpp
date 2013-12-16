#ifndef _PELI_HPP
#define _PELI_HPP

#include <vector>
#include <stdexcept>
#include "apuvalineet.hpp"
#include "lentokone.hpp"
#include "lentokentta.hpp"
#include "navipiste.hpp"

/*
MVC Model

Peli class maintains the game's runtime state and provides helper methods to update the state. Nothing else.
See PeliController and PeliView for more functionality.
*/

class Peli {
public:
	Peli(Ohjelma &o) : ohjelma(o), ajan_muutos(0.02f), koska_uusi_kone(1)
    {
		atis.lahtokiitotie = -1;
		atis.laskukiitotie = -1;
		atis.siirtopinta = -1;
		atis.ok = false;
    }

    Ohjelma& ohjelma;

    float ajan_muutos;
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
	std::vector <int> poistettavat;

	struct selvitys {
		int kone_id;
		std::string nimi;
		int toiminto;
		float aika;
	};

	std::vector <selvitys> selvitykset;

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

    int toiminto;
    int koska_uusi_kone;
    int koska_metar;
    int porrastusvirheet;
    int muut_virheet;

    struct Atis {
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


	enum virheet {VIRHE_KORKEUS_ALA = 1, VIRHE_KORKEUS_YLA, VIRHE_NOPEUS_ALA, VIRHE_NOPEUS_YLA, VIRHE_LAHESTYMISNOPEUS, VIRHE_LAHESTYMISKORKEUS, VIRHE_LAHESTYMISSUUNTA, VIRHE_LASKU, VIRHE_OIKOTIE, VIRHE_EI_VALITTUA_KONETTA, VIRHE_PORRASTUS, VIRHE_ALUEELTA};
	enum lukeminen {SUUNTA=1, NOPEUS, KORKEUS, TYHJENNA, LAHESTYMIS, OIKOTIE, ODOTUS, POIS, TYHJA = 0};
	enum tyyppi {SAAPUVA = 0, LAHTEVA = 1};
	enum atis_toiminnot {LAHTO, LASKU, SIIRTOPINTA};
	void luo_kone(Ohjelma& ohjelma);
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
    } metar;

	void aseta_virhe(int virhe);
};

#endif
