#include "ohjelma.hpp"
#include "valikko.hpp"
#include "peli.hpp"
#include "peliview.hpp"
#include "pelicontroller.hpp"
#include "kieli.hpp"
#include <stdexcept>


// The following undef required due to some conflict with VS+SDL.' To be removed later...
#ifdef _MSC_VER
#undef main
#endif

int main(int argc, char** argv) {
	try {
		Kieli kieli("fi_FI");
		Asetukset asetukset;
		Ohjelma ohjelma(asetukset);
		Peli peli(asetukset, ohjelma, kieli);
		PeliView view(asetukset, peli, ohjelma, kieli);
		PeliController controller(peli, view, ohjelma, asetukset, kieli);
		valikko valikko(ohjelma, view);

		while (true) {
			switch (valikko.aja()) {
			case valikko::PELI:
				controller.aja();
				continue;
			case valikko::LOPETUS:
				return 0;
			default:
				throw std::logic_error("Virheellinen tilanne valikossa!");
			}
		}
		return 0;
	}
	catch (std::exception& e) {
		std::cout << "Pieleen meni!" << std::endl;
		std::cout << e.what() << std::endl;
		return 1;
	}
}