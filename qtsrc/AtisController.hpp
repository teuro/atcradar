#ifndef ATISCONTROLLER_HPP
#define ATISCONTROLLER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "../src/apuvalineet.hpp"

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

namespace Atis {
    extern std::string lahtokiitotie;
    extern std::string laskukiitotie;
    extern int siirtokorkeus;
    extern int siirtopinta;

    void downloadPrressureLimit(std::string file, int siirtokorkeus);
    int calculateTL(int pressure);
    extern std::vector <paineraja> painerajat;
}

#endif // ATISCONTROLLER_HPP
