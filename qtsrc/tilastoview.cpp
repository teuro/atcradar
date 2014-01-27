#include "tilastoview.hpp"

void TilastoView::piirra(IPiirtoPinta& pinta) {
    pinta.kirjoita_tekstia("Tilastoja pelistä", 300, 20);
    int y = 60;
    int x = 200;
    int vali = 50;

    pinta.kirjoita_tekstia("Tunnus", x, y-20);
    pinta.kirjoita_tekstia("Sisään", x+vali*2, y-20);
    pinta.kirjoita_tekstia("Pois", x+vali*3, y-20);
    pinta.kirjoita_tekstia("Selvitykset", x+vali*4, y-20);

    for (std::vector <Peli::tilasto> :: iterator it = peli.ajat.begin(); it != peli.ajat.end(); ++it) {
        pinta.kirjoita_tekstia(it->tunnus, x, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->sisaan), x+vali*2, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->pois), x+vali*3, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->selvitykset), x+vali*4, y);
        y += 20;
    }
}
