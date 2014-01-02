#include "tilastoview.hpp"
#include <iostream>

void TilastoView::piirra(IPiirtoPinta& piirtopinta) {
	std::clog << "TilastoView::piirra()" << std::endl;

	piirtopinta.piirra_kuva("kuvat/tausta_tilasto.png", 0, 0);

	int y = 30;
	int x = 200;

	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_KUTSUTUNNUS), x, y - 15);
	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_ALUEELLE), x + 100, y - 15);
	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_POIS), x + 200, y - 15);
	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_ALUEELLA), x + 300, y - 15);
	piirtopinta.kirjoita_tekstia(kieli.anna_teksti(Kieli::TEKSTI_SELVITYKSET), x + 400, y - 15);

	double ka_alueella = 0;
	double ka_alueelta = 0;
	double ka_selvitykset = 0;
	double ka_alueelle = 0;

	for (unsigned int i = 0; i < peli.ajat.size(); ++i) {
		double alueella = peli.ajat[i].pois - peli.ajat[i].sisaan;
		ka_alueella += alueella;
		ka_selvitykset += peli.ajat[i].selvitykset;
		if (i > 0) {
			ka_alueelle += peli.ajat[i - 1].sisaan;
			ka_alueelta += peli.ajat[i - 1].pois;
		}

		piirtopinta.kirjoita_tekstia(peli.ajat[i].tunnus, x, y);
		piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].sisaan).c_str(), x + 100, y);
		piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].pois).c_str(), x + 200, y);
		piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].pois - peli.ajat[i].sisaan).c_str(), x + 300, y);
		piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat[i].selvitykset).c_str(), x + 400, y);

		y += 15;
	}

	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(peli.ajat.size()), x, y);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(ka_alueelle / peli.ajat.size()), x + 100, y);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(ka_alueelta / peli.ajat.size()), x + 200, y);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(ka_alueella), x + 300, y);
	piirtopinta.kirjoita_tekstia(apuvalineet::tekstiksi(ka_selvitykset / peli.ajat.size()), x + 400, y);

	piirtopinta.flip();
}
