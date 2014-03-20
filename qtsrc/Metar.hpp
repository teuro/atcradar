#ifndef METAR_HPP
#define METAR_HPP

#include <QString>
#include "apuvalineet.hpp"
#include <vector>
#include <fstream>
#include <map>
#include <stdexcept>

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
    QString getMessage() {
        return QString(
            ("EFRO " + apuvalineet::muuta_pituus(apuvalineet::tekstiksi(tuuli), 3) + 
            apuvalineet::muuta_pituus(apuvalineet::tekstiksi(voimakkuus), 2) + "KT Q" + 
            apuvalineet::tekstiksi(paine) + " " + apuvalineet::tekstiksi(nakyvyys) + " " + 
            apuvalineet::tekstiksi(lampotila) + "/" + apuvalineet::tekstiksi(kastepiste) + " " + pilvet).c_str());
    }

    int anna_tuuli();
    int anna_voimakkuus();
    int anna_paine();
    int anna_nakyvyys();
    int anna_lampotila();
    int anna_ilmankosteus();
    int anna_kastepiste();
    std::string anna_pilvet();

    void aseta_tuuli(int tuuli);
    void aseta_voimakkuus(int voimakkuus);
    void aseta_paine(int paine);
    void aseta_nakyvyys(int nakyvyys);
    void aseta_lampotila(int lampotila);
    void aseta_ilmankosteus(int ilmankosteus);
    void aseta_kastepiste(int kastepiste);
    void aseta_pilvet(std::string pilvet);

    void aseta_tila();
};

#endif // METAR_HPP
