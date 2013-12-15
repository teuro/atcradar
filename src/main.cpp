#include "ohjelma.hpp"
#include "valikko.hpp"
#include "peli.hpp"
#include "peliview.hpp"
#include "pelicontroller.hpp"
#include <stdexcept>

int main(int argc, char** argv)
{
	try {
		Ohjelma ohjelma;
		Peli peli(ohjelma);
		PeliView view(peli, ohjelma);
		PeliController controller(peli, view, ohjelma);
		valikko valikko(ohjelma, view);

		while (true)
		{
			static int pelin_tulos;
			switch (valikko.aja(pelin_tulos)) {
			case valikko::PELI:
				pelin_tulos = controller.aja();
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