#include "apuvalineet.hpp"
#include "ohjelma.hpp"
#include <cstdlib>

namespace apuvalineet {
	static double PII = 3.1415927;
}

double apuvalineet::px2nm(double px) {
	return px / (ohjelma::anna_asetus("ruutu_leveys") / ohjelma::anna_asetus("matka_vaaka"));
}

double apuvalineet::nm2px(double nm) {
	return nm * (ohjelma::anna_asetus("ruutu_leveys") / ohjelma::anna_asetus("matka_vaaka"));
}

double apuvalineet::deg2rad(double deg) {
	return (PII * deg) / 180.0;
}

double apuvalineet::rad2deg(double rad) {
	double deg = (rad * 180.0) / PII;

	return deg;
}
int apuvalineet::arvo_luku(int ala, int yla){
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
	double kulma = (apuvalineet::rad2deg(std::atan2((a.y - b.y), (a.x - b.x))) -90.0);

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