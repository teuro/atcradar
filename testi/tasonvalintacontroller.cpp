#include "tasonvalintacontroller.hpp"
#include "lukija.hpp"

int TasonValintaController::aja()
{
	int taso = -1;
	//syotteenluku lukija;

	while (taso < 0) {
		ohjelma.lue_syote();
		teksti = ohjelma.anna_viesti();

		if (ohjelma.lue_nappi(Ohjelma::NAPPI_ENTER)) {
			if (ohjelma.anna_viesti().length()) {
				taso = apuvalineet::luvuksi<int>(ohjelma.anna_viesti());
			}
		}

		view.piirra(piirtopinta);
	}

	switch (taso) {
		case 3:
			asetukset.muuta_asetusta("koska_uusi_ala", 30);
			asetukset.muuta_asetusta("koska_uusi_ala", 30);
			asetukset.muuta_asetusta("koska_uusi_yla", 80);
			asetukset.muuta_asetusta("maks_konemaara", 30);
			asetukset.muuta_asetusta("vaadittavat_kasitellyt", 30);
			break;
		case 2:
			asetukset.muuta_asetusta("koska_uusi_ala", 60);
			asetukset.muuta_asetusta("koska_uusi_yla", 120);
			asetukset.muuta_asetusta("maks_konemaara", 15);
			asetukset.muuta_asetusta("vaadittavat_kasitellyt", 20);
			break;
		case 1:
			asetukset.muuta_asetusta("koska_uusi_ala", 120);
			asetukset.muuta_asetusta("koska_uusi_yla", 240);
			asetukset.muuta_asetusta("maks_konemaara", 10);
			asetukset.muuta_asetusta("vaadittavat_kasitellyt", 15);
			break;
		case 0:
			asetukset.muuta_asetusta("koska_uusi_ala", 150);
			asetukset.muuta_asetusta("koska_uusi_yla", 300);
			asetukset.muuta_asetusta("maks_konemaara", 8);
			asetukset.muuta_asetusta("vaadittavat_kasitellyt", 10);
			break;
	}

	return taso;
}
