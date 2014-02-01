#include "tilastoview.hpp"

void TilastoView::piirra(IPiirtoPinta& pinta) {
    pinta.kirjoita_tekstia("Tilastoja pelistä", 300, 20);
    int y = 80;
    int x = 20;
    int vali = 65;
    double pistesumma = 0;
    double sisaan = 0;
    double pois = 0;
    double alueella = 0;
    double selvitykset = 0;

    std::vector <std::string> otsikot;

    otsikot.push_back("Tunnus");
    otsikot.push_back("Alueelle");
    otsikot.push_back("Alueelta");
    otsikot.push_back("Alueella");
    otsikot.push_back("Selvitykset");
    otsikot.push_back("Pisteet");

    for (unsigned int i = 0; i < otsikot.size(); ++i) {
        pinta.kirjoita_tekstia(otsikot[i], x+vali*i, y-25);
    }

    pinta.lineColor(x-5, y-20, x+vali*otsikot.size() + 5, y-20, 1);

    for (std::vector <Peli::tilasto> :: iterator it = peli.ajat.begin(); it != peli.ajat.end(); ++it) {
        pinta.kirjoita_tekstia(it->tunnus, x, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->sisaan), x+vali, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->pois), x+vali*2, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->pois - it->sisaan), x+vali*3, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->selvitykset), x+vali*4, y);
        pinta.kirjoita_tekstia(apuvalineet::tekstiksi(it->pisteet), x+vali*5, y);

        y += 20;

        pistesumma += it->pisteet;
        sisaan += it->sisaan;
        pois += it->pois;
        alueella += it->pois - it->sisaan;
        selvitykset += it->selvitykset;

        pinta.lineColor(x-5, y-15, x+vali*otsikot.size() + 5, y-15, 1);
    }

    pinta.kirjoita_tekstia("Summa", x, y);

    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(sisaan), x+vali, y);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(pois), x+vali*2, y);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(alueella), x+vali*3, y);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(selvitykset), x+vali*4, y);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(pistesumma), x+vali*5, y);

    /* Vasen reuna */
    pinta.lineColor(x-5, y+5+20, x-5, y-(peli.ajat.size()+2) * 20 - 5, 1);

    /* Oikea reuna */
    pinta.lineColor(x+vali*otsikot.size() + 5, y+5+20, x+vali*otsikot.size() + 5, y-(peli.ajat.size()+2) * 20 - 5, 1);

    /* Yläreuna */
    pinta.lineColor(x-5, y-(peli.ajat.size()+2) * 20 - 5, x+vali*otsikot.size() + 5, y-(peli.ajat.size()+2) * 20 - 5, 1);

    /* Alareuna */
    pinta.lineColor(x-5, y+5, x+vali*otsikot.size() + 5, y + 5, 1);

    pinta.kirjoita_tekstia("Keskiarvo", x, y+20);

    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(sisaan / peli.ajat.size()), x+vali, y+20);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(pois / peli.ajat.size()), x+vali*2, y+20);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(alueella / peli.ajat.size()), x+vali*3, y+20);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(selvitykset / peli.ajat.size()), x+vali*4, y+20);
    pinta.kirjoita_tekstia(apuvalineet::tekstiksi(pistesumma / peli.ajat.size()), x+vali*5, y+20);

    pinta.lineColor(x-5, y+5+20, x+vali*otsikot.size() + 5, y + 5 + 20, 1);

    for (unsigned int i = 1; i < otsikot.size(); ++i) {
        pinta.lineColor(x + vali * i - 5, y-(peli.ajat.size()+2) * 20 - 5, x + vali * i - 5, y+5+20, 1);
    }
}
