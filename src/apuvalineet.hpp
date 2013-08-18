#ifndef _APUVALINEET_HPP#define _APUVALINEET_HPP

#include <cmath>
#include <iostream>
#include <sstream>
namespace apuvalineet {
	extern double zoom;

	class piste {
	public:
		double x;
		double y;
	};

	struct vektori {
		piste alku;
		piste loppu;

		double pituus;
		double suunta;
	};

	double nm2px(double nm);
	double px2nm(double px);
	double deg2rad(double deg);
	double rad2deg(double rad);
	double mi2ft(double mi);
	double ft2mi(double ft);
	int arvo_luku(int ala, int yla);

	template <class T>
	std::string tekstiksi(T a) {
		std::stringstream os;

		os << a;

		return os.str();
	}

	template <class T>
	T luvuksi(std::string a) {
		T b;
		std::stringstream os;

		os << a;

		os >> b;

		return b;
	}

	double etaisyys(const piste& a, const piste& b);
	piste uusi_paikka(const piste& a, double suunta, double pituus);
	vektori suunta_vektori(const piste& a, const piste& b);
	double laske_kulma(const piste& a, const piste& b);
	std::string muuta_pituus(std::string alkuperainen, int haluttupituus);
}
#endif
