#include "atiscontroller.hpp"

int AtisController::aja() {
	Peli::Atis &atis = peli.atis;
	atis.lue_paineet("data/painerajat.txt");
	int toiminto = Peli::LAHTO;
	atis.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_ANNA_LAHTOKIITOTIE);
	ohjelma.tyhjenna_viesti();

	while (atis.ok == false) {
		peli.syote = ohjelma.lue_syote();
		ohjelma.odota(20);

		if (ohjelma.lue_nappi(Ohjelma::NAPPI_F5)) {
			toiminto = Peli::LAHTO;
		}
		else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F7)) {
			toiminto = Peli::LASKU;
		}
		else if (ohjelma.lue_nappi(Ohjelma::NAPPI_F8)) {
			toiminto = Peli::SIIRTOPINTA;
		}

		if (ohjelma.anna_viesti().length() > 1 && ohjelma.lue_nappi(Ohjelma::NAPPI_ENTER)) {

			std::vector <kiitotie>::iterator tmp;
			size_t index;

			switch (toiminto) {
			case Peli::LAHTO:
				tmp = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), ohjelma.anna_viesti());
				index = std::distance(peli.kentta.kiitotiet.begin(), tmp);

				if (tmp != peli.kentta.kiitotiet.end()) {
					atis.lahtokiitotie = index;
					atis.lahto = ohjelma.anna_viesti();
					ohjelma.tyhjenna_viesti();
				}

				toiminto = Peli::LASKU;
				peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_ANNA_LASKUKIITOTIE);
				break;
			case Peli::LASKU:
				tmp = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), ohjelma.anna_viesti());
				index = std::distance(peli.kentta.kiitotiet.begin(), tmp);

				if (tmp != peli.kentta.kiitotiet.end()) {
					atis.laskukiitotie = index;
					atis.lasku = ohjelma.anna_viesti();
					ohjelma.tyhjenna_viesti();
				}

				toiminto = Peli::SIIRTOPINTA;
				peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_ANNA_SIIRTOPINTA);

				break;
			case Peli::SIIRTOPINTA:
				atis.siirtopinta = apuvalineet::luvuksi<int>(ohjelma.anna_viesti());
				ohjelma.tyhjenna_viesti();

				break;
			}
		}

		bool siirto_ok = false;
		bool lahto_ok = false;
		bool lasku_ok = false;

		view.piirra();

		if (atis.lahtokiitotie > -1 && atis.laskukiitotie > -1 && atis.siirtopinta > -1) {
			peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_ONKO_ATIS_OK);

			double vastakomponentti_lahto = std::cos(apuvalineet::deg2rad(peli.metar.tuuli) - apuvalineet::deg2rad(peli.kentta.kiitotiet[atis.lahtokiitotie].suunta)) * peli.metar.voimakkuus;
			double vastakomponentti_lasku = std::cos(apuvalineet::deg2rad(peli.metar.tuuli) - apuvalineet::deg2rad(peli.kentta.kiitotiet[atis.laskukiitotie].suunta)) * peli.metar.voimakkuus;

			double siirtopinta = atis.etsi_siirtopinta(peli.metar.paine);

			double max_vasta = 0;
			double vasta;

			for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
				vasta = std::cos(apuvalineet::deg2rad(peli.metar.tuuli) - apuvalineet::deg2rad(peli.kentta.kiitotiet[i].suunta)) * peli.metar.voimakkuus;

				if (vasta > max_vasta) {
					max_vasta = vasta;
				}
			}

			if (siirtopinta == atis.siirtopinta) {
				siirto_ok = true;
			}
			else {
				peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_SIIRTOPINTA_VAARIN) + apuvalineet::tekstiksi(siirtopinta);
				toiminto = Peli::SIIRTOPINTA;
			}

			if (vastakomponentti_lahto > -0.1) {
				lahto_ok = true;
			}
			else {
				peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_LAHTOBAANA_VAARIN);
				toiminto = Peli::LAHTO;
			}

			if (vastakomponentti_lasku > -0.1) {
				lasku_ok = true;
			}
			else {
				peli.ohje = kieli.anna_teksti(Kieli::TEKSTI_OHJE_LASKUBAANA_VAARIN);
				toiminto = Peli::LASKU;
			}
		}

		if (siirto_ok && lahto_ok && lasku_ok) {
			atis.ok = true;
			atis.siirtokorkeus = 5000;
		}
	}
	return 0;
}
