#ifndef ATISCONTROLLER_HPP
#define ATISCONTROLLER_HPP

#include <QObject>

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "apuvalineet.hpp"

class paineraja {
public:
    int alaraja;
    int ylaraja;
    int siirtopinta;

    paineraja (int ar, int yr, int sp) {
        this->alaraja = ar;
        this->ylaraja = yr;
        this->siirtopinta = sp;
    }
};

class Atis {
    std::string lahtokiitotie;
    std::string laskukiitotie;
    int siirtokorkeus;
    int siirtopinta;
public:
    Atis() {
        lahtokiitotie = "";
        laskukiitotie = "";
        siirtokorkeus = -1;
        siirtopinta = -1;
    }

    void aseta_lahtokiitotie(std::string lk) { this->lahtokiitotie = lk; }
    void aseta_laskukiitotie(std::string lk) { this->laskukiitotie = lk; }
    void aseta_siirtopinta(int sp) { this->siirtopinta = sp; }
    void aseta_siirtokorkeus(int sk) { this->siirtokorkeus = sk; }

    std::string anna_lahtokiitotie() { return this->lahtokiitotie; }
    std::string anna_laskukiitotie() { return this->laskukiitotie; }
    int anna_siirtokorkeus() { return this->siirtokorkeus; }
    int anna_siirtopinta() { return this->siirtopinta; }

    void lataa_painerajat(std::string tiedosto, int siirtokorkeus);
    int laske_siirtopinta(int paine);

    void tyhjenna() { painerajat.clear(); }

    std::vector <paineraja> painerajat;
};

#endif // ATISCONTROLLER_HPP
