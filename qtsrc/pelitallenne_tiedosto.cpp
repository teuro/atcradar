#include "pelitallenne_tiedosto.hpp"

void TiedostoPeliTallenne::tallenna() {
    std::ofstream ulos("pelit/pelinnimi.txt", std::ios::out);

    if (!ulos) {
        std::clog << "File ei aukea!";
    }

    for (std::list <lentokone*> :: iterator lk = peli.koneet.begin(); lk != peli.koneet.end(); ++lk) {
        ulos << (*lk)->anna_kutsutunnus() << "|"
             << (*lk)->paikka.x << "|"
             << (*lk)->paikka.y << "|"
             << (*lk)->anna_korkeus() << "|"
             << (*lk)->anna_suunta() << "|"
             << (*lk)->anna_tyyppi() << "|"
             << (*lk)->anna_odotus()
             << "|" << std::endl;
    }

    ulos.close();
}

void TiedostoPeliTallenne::lataa() {

}

TiedostoPeliTallenne::TiedostoPeliTallenne(Peli& p) : IPeliTallenne(p) { }
