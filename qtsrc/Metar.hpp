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
    Metar() {
        tuuli           = apuvalineet::pyorista(apuvalineet::arvo_luku(0, 360), 5);
        voimakkuus      = apuvalineet::arvo_luku(0, 20);
        paine           = apuvalineet::arvo_luku(960, 1050);
        nakyvyys        = apuvalineet::pyorista(apuvalineet::arvo_luku(200, 9999), 100);
        lampotila       = apuvalineet::arvo_luku(-30, 60);
        ilmankosteus    = apuvalineet::arvo_luku(50, 100);
        kastepiste      = (lampotila - (100 - ilmankosteus)) / 5;

        std::vector <std::string> pilvityypit;

        pilvityypit.push_back("SKC");
        pilvityypit.push_back("FEW");
        pilvityypit.push_back("BKN");
        pilvityypit.push_back("SCT");
        pilvityypit.push_back("OVC");

        for (int i = 0; i < apuvalineet::arvo_luku(0, 5); ++i) {
            pilvet += pilvityypit[apuvalineet::arvo_luku(0, pilvityypit.size()-1)] + apuvalineet::tekstiksi(apuvalineet::pyorista(apuvalineet::arvo_luku(1000, 8800), 100)) + " ";
        }
    }

    QString getMessage() {
        return QString::fromStdString("EFRO " + apuvalineet::muuta_pituus(apuvalineet::tekstiksi(tuuli), 3) + apuvalineet::muuta_pituus(apuvalineet::tekstiksi(voimakkuus), 2) + "KT Q" + apuvalineet::tekstiksi(paine) + " " + apuvalineet::tekstiksi(nakyvyys) + " " + apuvalineet::tekstiksi(lampotila) + "/" + apuvalineet::tekstiksi(kastepiste) + " " + pilvet);
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
    std::string aseta_pilvet(std::string pilvet) { this->pilvet = pilvet;}
};

#endif // METAR_HPP
