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
		std::string kieli_tunnus = "fi_FI";
		
		if (argc > 1) {
			kieli_tunnus = std::string(argv[1]);
		} 
		
		Kieli kieli(kieli_tunnus);
		
		Asetukset asetukset;
		Ohjelma ohjelma(asetukset);
		Peli peli(asetukset, ohjelma, kieli);
		PeliView view(asetukset, peli, ohjelma, kieli);
		PeliController controller(peli, view, ohjelma, asetukset, kieli);
		
		valikko valikko(ohjelma, view);

		valikko.lisaa_kohta(1, "Peli");
		valikko.lisaa_kohta(0, "Lopeta");
		
		while (true) {
			switch (valikko.aja()) {
			case 1:
				controller.aja();
				continue;
			case 0:
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