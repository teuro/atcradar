#include "apuvalineet.hpp"

static double PII = 3.1415927;

namespace apuvalineet {
    int ruutu_leveys = 900;
    const int matka_vaaka = 80;
}

double apuvalineet::px2nm(double px) {
	return px / (ruutu_leveys / matka_vaaka);
}

double apuvalineet::nm2px(double nm) {
	return nm * (ruutu_leveys / matka_vaaka);
}

double apuvalineet::deg2rad(double deg) {
   return (PII * deg) / 180.0;
}

double apuvalineet::rad2deg(double rad) {
	double deg = (rad * 180.0) / PII;

	return deg;
}
int apuvalineet::arvo_luku(int ala, int yla) {
    if (yla == 0) {
        throw std::logic_error("Jako nollalla!");
    }
    int tmp_luku;

    tmp_luku = ala + std::rand() % (yla - ala);

    return tmp_luku;
}

double apuvalineet::etaisyys(const piste& a, const piste& b) {
	double x = std::abs((a.x - b.x));
	double y = std::abs((a.y - b.y));

    double matka = px2nm(std::sqrt((x * x) + (y * y)));

	return matka;
}

double apuvalineet::laske_kulma(const piste& a, const piste& b) {
	double kulma = apuvalineet::rad2deg(std::atan2((a.y - b.y), (a.x - b.x))) -90.0;

	if (kulma < 0) {
		kulma += 360;
	} else if (kulma > 360) {
		kulma -= 360;
	}

	return kulma;
}


apuvalineet::vektori apuvalineet::suunta_vektori(const apuvalineet::piste& a, const apuvalineet::piste& b) {
	apuvalineet::vektori tmp;

	tmp.alku = a;
	tmp.loppu = b;

	tmp.pituus = etaisyys(tmp.alku, tmp.loppu);
	tmp.suunta = laske_kulma(tmp.alku, tmp.loppu);

	return tmp;
}

apuvalineet::piste apuvalineet::uusi_paikka(const apuvalineet::piste& a, double suunta, double pituus) {
	apuvalineet::piste b;

	if (suunta < 0) {
		suunta += 360;
	} else if (suunta > 360) {
		suunta -= 360;
	}

	b.x = a.x + std::cos(deg2rad(suunta - 90.0)) * apuvalineet::nm2px(pituus);
	b.y = a.y + std::sin(deg2rad(suunta - 90.0)) * apuvalineet::nm2px(pituus);

	return b;
}

double apuvalineet::mi2ft(double mi) {
	return mi * 5280;
}

double apuvalineet::ft2mi(double ft) {
	return ft / 5280;
}

std::string apuvalineet::muuta_pituus(std::string alkuperainen, int haluttu_pituus) {
	int nollia = haluttu_pituus - alkuperainen.length();
	std::string tmp;

	for (int i = 0; i < nollia; ++i) {
		tmp += "0";
	}

	tmp += alkuperainen;

	return tmp;
}

int apuvalineet::pyorista(double luku, int jaollisuus) {
	return std::floor(luku/jaollisuus) * jaollisuus;
}

std::vector <std::string> apuvalineet::pilko_rivi(std::string rivi, std::string erotin) {
	size_t pos = 0;
	std::vector <std::string> asiat;
	std::string sana;

    while ((pos = rivi.find(erotin)) != std::string::npos) {
		sana = rivi.substr(0, pos);
		asiat.push_back(sana);
		rivi.erase(0, pos + erotin.length());
	}

	return asiat;
}

bool apuvalineet::onko_alueella(const piste& a, const piste& b, double sade) {
    double valimatka = etaisyys(a, b);

	return valimatka < (2 * sade);
}

double apuvalineet::laske_vastatuuli(double kiitotie, double tuuli) {
    return (std::cos(deg2rad(std::abs((tuuli + 180) - kiitotie))));
}

double apuvalineet::laske_sivutuuli(double kiitotie, double tuuli) {
    return (std::sin(deg2rad(std::abs((tuuli + 180) - kiitotie))));
}

std::string apuvalineet::muotoile_aika(std::string formaatti, double aika) {
    int sekunnit = aika;

    int tunnit = sekunnit / 3600;
    sekunnit -= tunnit * 3600;
    int minuutit = sekunnit / 60;
    sekunnit -= minuutit * 60;

    std::map <std::string, int> formaatit;

    formaatit["%h"] = tunnit;
    formaatit["%m"] = minuutit;
    formaatit["%s"] = sekunnit;
    formaatit["%ms"] = sekunnit * 1000;

    for (std::map <std::string, int> :: iterator it = formaatit.begin(); it != formaatit.end(); ++it) {
        unsigned int kohta = formaatti.find(it->first);

        if (kohta != std::string::npos) {
            formaatti.replace(formaatti.find(it->first, 0), 2, apuvalineet::muuta_pituus(apuvalineet::tekstiksi(it->second), 2));
        }
    }

    return formaatti;
}

std::vector <std::string> apuvalineet::lue_tiedosto(std::string tiedosto) {
    std::vector <std::string> tmp;

    std::ifstream sisaan(tiedosto.c_str(), std::ios::in);
    std::string rivi;

    if (!sisaan) {
        throw std::runtime_error("Tiedosto " + tiedosto + " ei aukea tai se puuttuu ");
    }

    while (std::getline(sisaan, rivi)) {
        tmp.push_back(rivi);
    }

    sisaan.close();

    return tmp;
}
