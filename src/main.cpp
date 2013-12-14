#ifdef WIN32 
#pragma comment(lib, "SDL.lib") 
#pragma comment(lib, "SDLmain.lib") 
#endif 

// main.cpp
#include "ohjelma.hpp"
#include "valikko.hpp"
#include "peli.hpp"
#include <stdexcept>

int main(int argc, char** argv)
	try {
		Ohjelma ohjelma;
		Peli peli(ohjelma);
		PeliView view(peli, ohjelma);
		PeliController controller(peli, view, ohjelma);
		valikko valikko(ohjelma, view);

		ohjelma.alku();
		view.alku();

		while (true)
		{
			static int pelin_tulos;
			switch (valikko.aja(pelin_tulos)) {
			case valikko::PELI:
				pelin_tulos = controller.aja();
				continue;
			case valikko::LOPETUS:
				break;
			}
			throw std::logic_error("Virheellinen tilanne valikossa!");
		}
		ohjelma.loppu();
		return 0;

	}
	catch (std::exception& e) {
		std::cout << "Pieleen meni!" << std::endl;
		std::cout << e.what() << std::endl;
		return 1;
	}
