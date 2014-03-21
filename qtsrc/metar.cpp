#include "metar.hpp"

void Metar::aseta_tila() {
    this->aseta_kastepiste(this->lampotila - ((100 - this->ilmankosteus) / 5));

    this->aseta_voimakkuus(std::abs(1013 - this->paine) * 1.75);

    int pilvia = apuvalineet::arvo_luku(0, 4 - (this->lampotila - this->kastepiste));
    std::string pilvi;

    std::vector <std::string> tyypit = apuvalineet::lue_tiedosto("data/pilvet.txt");

    for (int i = 0; i < pilvia; ++i) {
        pilvi += tyypit[apuvalineet::arvo_luku(0, tyypit.size())] +apuvalineet::tekstiksi(apuvalineet::pyorista(apuvalineet::arvo_luku(500, 8000), 100)) + " ";
    }

    this->aseta_pilvet(pilvi);
    //Paine kasvaa yhdellä näkyvyys 136
    this->nakyvyys = apuvalineet::pyorista(3200 + (this->paine - 980) * 136, 100);

    if (this->nakyvyys > 9999) {
        this->nakyvyys = 9999;
    }
}

int Metar::anna_tuuli() {return this->tuuli; }
int Metar::anna_voimakkuus() {return this->voimakkuus; }
int Metar::anna_paine() {return this->paine; }
int Metar::anna_nakyvyys() {return this->nakyvyys; }
int Metar::anna_lampotila() {return this->lampotila; }
int Metar::anna_ilmankosteus() {return this->ilmankosteus; }
int Metar::anna_kastepiste() {return this->kastepiste; }
std::string Metar::anna_pilvet() { return this->pilvet; }

void Metar::aseta_tuuli(int tuuli) {this->tuuli = tuuli; }
void Metar::aseta_voimakkuus(int voimakkuus) {this->voimakkuus = voimakkuus;}
void Metar::aseta_paine(int paine) {this->paine = paine;}
void Metar::aseta_nakyvyys(int nakyvyys) {this->nakyvyys = nakyvyys;}
void Metar::aseta_lampotila(int lampotila) {this->lampotila = lampotila;}
void Metar::aseta_ilmankosteus(int ilmankosteus) {this->ilmankosteus = ilmankosteus;}
void Metar::aseta_kastepiste(int kastepiste) {this->kastepiste = kastepiste;}
void Metar::aseta_pilvet(std::string pilvet) { this->pilvet = pilvet; }
