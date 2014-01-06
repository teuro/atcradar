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
        tuuli       = apuvalineet::arvo_luku(3, 23);
        voimakkuus  = apuvalineet::arvo_luku(0, 20);
        paine       = apuvalineet::arvo_luku(940, 1060);
    }

    QString getMessage() {
        return QString("EFRO 13525KT");
    }
};

#endif // METAR_HPP
