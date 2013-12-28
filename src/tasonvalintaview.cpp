#include "tasonvalintaview.hpp"

void TasonValintaView::piirra()
{
	piirtopinta.piirra_kuva("kuvat/tausta_valikko.png", 0, 0);

	std::string ohje = kieli.anna_teksti(Kieli::TEKSTI_VALITSE_TASO);

	piirtopinta.kirjoita_tekstia(ohje, 50, 30);
	piirtopinta.kirjoita_tekstia(teksti, 50, 50);

	piirtopinta.flip();
}
