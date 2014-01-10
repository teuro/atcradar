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

    int anna_tuuli() {return this->tuuli; }
    int anna_voimakkuus() {return this->voimakkuus; }
    int anna_paine() {return this->paine; }
    int anna_nakyvyys() {return this->nakyvyys; }
    int anna_lampotila() {return this->lampotila; }
    int anna_ilmankosteus() {return this->ilmankosteus; }
    int anna_kastepiste() {return this->kastepiste; }
    std::string anna_pilvet() { return this->pilvet; }

    void aseta_tuuli(int tuuli) {this->tuuli = tuuli; }
    void aseta_voimakkuus(int voimakkuus) {this->voimakkuus = voimakkuus;}
    void aseta_paine(int paine) {this->paine = paine;}
    void aseta_nakyvyys(int nakyvyys) {this->nakyvyys = nakyvyys;}
    void aseta_lampotila(int lampotila) {this->lampotila = lampotila;}
    void aseta_ilmankosteus(int ilmankosteus) {this->ilmankosteus = ilmankosteus;}
    void aseta_kastepiste(int kastepiste) {this->kastepiste = kastepiste;}
    void aseta_pilvet(std::string pilvet) { this->pilvet = pilvet; }
};

#endif // METAR_HPP
