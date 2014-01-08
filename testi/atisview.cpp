#include "atisview.hpp"

void AtisView::piirra(IPiirtoPinta& piirtopinta) {
	piirtopinta.piirra_kuva("kuvat/tausta_atis.png", 0, 0);

	piirtopinta.kirjoita_tekstia(peli.ohje, 50, 30);
	piirtopinta.kirjoita_tekstia(peli.syote, 50, 50);

	int y = 70;

	for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
		piirtopinta.kirjoita_tekstia(peli.kentta.kiitotiet[i].nimi, 400, y);
		y += 20;
	}

	piirra_metar(piirtopinta);
//	piirra_ohje(peli.ohje);
	piirtopinta.kirjoita_tekstia(peli.ohje.c_str(), 50, 30);

	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_OHJE_LAHTOKIITOTIE), 50, 90);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.atis.lahto), 150, 90);
	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_OHJE_LASKUKIITOTIE), 50, 110);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.atis.lasku), 150, 110);
	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_OHJE_SIIRTOPINTA), 50, 130);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.atis.siirtopinta), 150, 130);
	piirtopinta.flip();
	//	SDL_Flip(ruutu);
}

void AtisView::piirra_metar(IPiirtoPinta& piirtopinta) {
	std::string tuuli = apuvalineet::tekstiksi(peli.metar.tuuli);
	std::string voimakkuus = apuvalineet::tekstiksi(peli.metar.voimakkuus);
	std::string paine = apuvalineet::tekstiksi(peli.metar.paine);
	std::string nakyvyys = apuvalineet::tekstiksi(peli.metar.nakyvyys);
	std::string lampotila = apuvalineet::tekstiksi(peli.metar.lampotila);
	std::string kastepiste = apuvalineet::tekstiksi(peli.metar.kastepiste);
	std::string pilvet;

	for (std::map<std::string, int>::iterator it = peli.metar.pilvet.begin(); it != peli.metar.pilvet.end(); ++it) {
		pilvet += it->first + apuvalineet::tekstiksi(it->second) + " ";
	}

	tuuli = apuvalineet::muuta_pituus(tuuli, 3);
	voimakkuus = apuvalineet::muuta_pituus(voimakkuus, 2);

	piirtopinta.kirjoita_tekstia(peli.kentta.nimi + " " + tuuli + voimakkuus + "KT " + nakyvyys + " " + lampotila + " / " + kastepiste + " " + pilvet + " " + paine, 50, 10);
}
