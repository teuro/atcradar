#include "lentokentta.hpp"

kiitotie::kiitotie(std::string nimi, apuvalineet::piste alkupiste, double pituus, double suunta, double alkunousukorkeus, double alkunoususuunta, double lahestymiskorkeus, double lahestymispiste, double hidastuspiste) {
	this->alkupiste = alkupiste;
	this->pituus = pituus;
	this->suunta = suunta;

	this->alkunousukorkeus = alkunousukorkeus;
	this->alkunoususuunta = alkunoususuunta;
	this->nimi = nimi;
    this->lahestymiskorkeus = lahestymiskorkeus;

	this->loppupiste 		= apuvalineet::uusi_paikka(this->alkupiste, this->suunta, this->pituus);
	this->lahestymispiste 	= apuvalineet::uusi_paikka(this->alkupiste, this->suunta - 180.0, lahestymispiste/*Asetukset::anna_asetus("lahestymispiste")*/);
	this->hidastuspiste 	= apuvalineet::uusi_paikka(this->alkupiste, this->suunta - 180.0, hidastuspiste/*Asetukset::anna_asetus("hidastuspiste")*/);
	this->odotuspiste		= apuvalineet::uusi_paikka(this->alkupiste, this->suunta - 90.0, 0.15);
    this->lahestymiskorkeus = lahestymiskorkeus;
}
