#include <stdexcept>
#include <dirent.h>
#include "ohjelma.hpp"
#include "valikko.hpp"
#include "valikkoview.hpp"
#include "peli.hpp"
#include "peliview.hpp"
#include "pelicontroller.hpp"
#include "tilastoview.hpp"
#include "sdlpiirtopinta.hpp"
#include "tasonvalintacontroller.hpp"
#include "tasonvalintaview.hpp"
#include "kieli.hpp"

// The following undef required due to some conflict with VS+SDL.' To be removed later...
#ifdef _MSC_VER
#undef main
#endif

std::vector <std::string> lataa_tiedostojen_nimet(std::string kansio) {
	DIR *dir;
	struct dirent *ent;
	std::vector <std::string> tmp;

	if ((dir = opendir(kansio.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string name = std::string(ent->d_name);

			if (name.length() > 2) {
				tmp.push_back(name);
			}
		}
		closedir(dir);
	}

	return tmp;
}

void LataaKentat(ValikkoData& kentatData)
{
	std::vector <std::string> nimet = lataa_tiedostojen_nimet("kentat/");

	for (unsigned int i = 0; i < nimet.size(); ++i) {
		kentatData.lisaa_kohta(i, nimet[i]);
	}
}

int main(int argc, char** argv) {
	try {
		std::string kieli_tunnus = "fi_FI";
		
		if (argc > 1) {
			kieli_tunnus = std::string(argv[1]);
		} 
		
		Kieli kieli(kieli_tunnus);
		
		Asetukset asetukset;
		Ohjelma ohjelma(asetukset);
		SDLPiirtoPinta pinta(asetukset);
		
		ValikkoData valikkoData;
		valikkoData.lisaa_kohta(1, "Peli");
		valikkoData.lisaa_kohta(0, "Lopeta");

		ValikkoView valikkoView(valikkoData);
		valikko paaValikko(ohjelma, valikkoView, valikkoData, pinta);
		
		while (true) {
			// P‰‰valikko
			int valittu = paaValikko.aja();
			if (valittu == 0)
			{
				break;
			}

			// Tason valinta
			std::string tasoData;
			TasonValintaView tasonvalintaView(kieli, tasoData);
			TasonValintaController tasonvalinta(asetukset, kieli, ohjelma, tasonvalintaView, tasoData, pinta);
			tasonvalinta.aja();

			// Kent‰n valinta
			ValikkoData kentatData;
			ValikkoView kentatView(kentatData);
			valikko kentatValikko(ohjelma, kentatView, kentatData, pinta);
			LataaKentat(kentatData);

			int kentta_id = -1;
			while (kentta_id < 0) {
				kentta_id = kentatValikko.aja();
				std::clog << "Ladataan kentta " << kentatData.kohdat[kentta_id] << std::endl;
			}

			// Peli
			Peli peli(asetukset, ohjelma, kieli, kentatData.kohdat[kentta_id]);
			PeliView peliView(asetukset, peli, ohjelma, kieli);
			PeliController pelicontroller(peli, peliView, ohjelma, asetukset, kieli, pinta);
			pelicontroller.aja();

			// N‰yt‰ tilastot
			TilastoView tilastoView(kieli, peli);
			tilastoView.piirra(pinta);
			ohjelma.odota_nappi();
		}

		return 0;
	}
	catch (std::exception& e) {
		std::cout << "Pieleen meni!" << std::endl;
		std::cout << e.what() << std::endl;
		return 1;
	}
}