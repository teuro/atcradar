#include "apuvalineet.hpp"
#include "ohjelma.hpp"
#include <cstdlib>

namespace apuvalineet {
	static double PII = 3.1415927;
}

double apuvalineet::jana::pituus() {
	double x_jana = std::abs(alku.x - loppu.x);
	double y_jana = std::abs(alku.y - loppu.y);

	double pituus = std::sqrt(std::pow(x_jana, 2.0) + std::pow(y_jana, 2.0));

	return pituus;
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

	if (deg < 0) {
		deg += 360;
	} else if (deg > 360) {
		deg -= 360;
	}

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

double apuvalineet::kulma(const piste& a, const piste& b) {
	return (apuvalineet::rad2deg(std::atan2((b.y - a.y), (b.x - a.x))) -90.0);
}

apuvalineet::piste apuvalineet::uusi_paikka(const apuvalineet::piste& a, double suunta, double pituus) {
	apuvalineet::piste b;

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
