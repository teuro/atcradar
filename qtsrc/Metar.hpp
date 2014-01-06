#ifndef METAR_HPP
#define METAR_HPP

#include <QString>
#include "../src/apuvalineet.hpp"

class Metar {
    int tuuli;
    int voimakkuus;
    double paine;
    int nakyvyys;
    int lampotila;
    int ilmankosteus;
    int kastepiste;
    std::string pilvet;
public:
    Metar () {
        tuuli           = apuvalineet::pyorista(apuvalineet::arvo_luku(3, 23), 5);
        voimakkuus      = apuvalineet::arvo_luku(0, 20);
        paine           = apuvalineet::arvo_luku(940, 1060);
        nakyvyys        = apuvalineet::pyorista(apuvalineet::arvo_luku(200, 9999), 100);
        lampotila       = apuvalineet::arvo_luku(-30, 60);
        ilmankosteus    = apuvalineet::arvo_luku(50, 100);
        kastepiste      = (lampotila - (100 - ilmankosteus)) / 5;
        pilvet          = "BKN2700 SCT6800 OVC1100";
    }

    QString getMessage() {
        return QString::fromStdString("EFRO " + apuvalineet::muuta_pituus(apuvalineet::tekstiksi(tuuli), 3) + apuvalineet::muuta_pituus(apuvalineet::tekstiksi(voimakkuus), 2) + "KT Q" + apuvalineet::tekstiksi(paine) + " " + apuvalineet::tekstiksi(nakyvyys) + " " + apuvalineet::tekstiksi(lampotila) + "/" + apuvalineet::tekstiksi(kastepiste) + " " + pilvet);
    }
};

#endif // METAR_HPP
