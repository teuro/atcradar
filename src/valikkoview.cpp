#include "valikkoview.hpp"
#include <iostream>

void ValikkoView::piirra()
{
	std::clog << "ValikkoView::piirra(" << data.valittuRivi << ")" << std::endl;

	piirtopinta.piirra_kuva("kuvat/tausta_valikko.png", 0, 0);
	int y = 20;

	for (std::map <int, std::string> ::iterator it = data.kohdat.begin(); it != data.kohdat.end(); ++it) {
		if (it->first == data.valittuRivi) {
			piirtopinta.kirjoita_tekstia(it->second, 50, y, true);
		}
		else {
			piirtopinta.kirjoita_tekstia(it->second, 50, y);
		}
		y += 20;
	}

	// Laitetaan piirustukset esille.
	piirtopinta.flip();
}
