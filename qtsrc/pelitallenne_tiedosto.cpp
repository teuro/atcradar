#include "pelitallenne_tiedosto.hpp"

void TiedostoPeliTallenne::tallenna() {
    std::clog << peli.koneet.size() << std::endl;
}

void TiedostoPeliTallenne::lataa() {

}

TiedostoPeliTallenne::TiedostoPeliTallenne(Peli& p) : IPeliTallenne(p) { }
