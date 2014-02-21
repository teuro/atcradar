#include "AtisController.hpp"

void Atis::lataa_painerajat(std::string tiedosto, int siirtokorkeus) {
    std::ifstream in(tiedosto.c_str(), std::ios::in);

    if (!in) {
        throw std::runtime_error(QObject::tr("Tieddostoa painerajat.txt ei aukea tai se puuttuu").toStdString());
    }

    std::string line;
    std::vector <std::string> words;
    int place = -1;

    while (std::getline(in, line)) {
        words = apuvalineet::pilko_rivi(line, "|");

        if (words[0] == "P" && apuvalineet::luvuksi<int>(words[1]) == siirtokorkeus) {
            place = apuvalineet::luvuksi<int>(words[2]);
        } else if (words[0] == "K" && place < 0) {
            return;
        } else if (words[0] == "K") {
            painerajat.push_back(paineraja(apuvalineet::luvuksi<int>(words[1]), apuvalineet::luvuksi<int>(words[2]), apuvalineet::luvuksi<int>(words[place])));
        }

        words.clear();
    }

    in.close();
}

int Atis::laske_siirtopinta(int pressure) {
    for (unsigned int i = 0; i < painerajat.size(); ++i) {
        if (pressure >= Atis::painerajat[i].alaraja && pressure <= Atis::painerajat[i].ylaraja) {
            return painerajat[i].siirtopinta;
        }
    }

    return 0;
}
