#ifndef METAR_HPP
#define METAR_HPP

#include <QString>
#include "../src/apuvalineet.hpp"

class Metar {
    int tuuli;
    int voimakkuus;
    double paine;
public:
    Metar () {
        tuuli       = apuvalineet::pyorista(apuvalineet::arvo_luku(3, 23), 5);
        voimakkuus  = apuvalineet::arvo_luku(0, 20);
        paine       = apuvalineet::arvo_luku(940, 1060);
    }

    QString getMessage() {
        return QString::fromStdString(apuvalineet::muuta_pituus(apuvalineet::tekstiksi(tuuli), 3) + apuvalineet::muuta_pituus(apuvalineet::tekstiksi(voimakkuus), 2) + "KT Q" + apuvalineet::tekstiksi(paine));
    }
};

#endif // METAR_HPP
