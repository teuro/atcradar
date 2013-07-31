// main.cpp
#include "ohjelma.hpp"
#include "valikko.hpp"
#include "peli.hpp"
#include <iostream>
#include <stdexcept>

static bool silmukka() {
	static int pelin_tulos;
	switch (valikko::aja(pelin_tulos)) {
		case valikko::PELI:
			pelin_tulos = peli::aja();
			return true;
		case valikko::LOPETUS:
			return false;
	}
	throw std::logic_error("Virheellinen tilanne valikossa!");
}

int main()
try {
	ohjelma::alku();
	while (silmukka());
	ohjelma::loppu();
	return 0;

} catch (std::exception& e) {
	std::cout << "Pieleen meni!" << std::endl;
	std::cout << e.what() << std::endl;
	return 1;
}
