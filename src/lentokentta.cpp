#include "asetukset.h"
#include "lentokentta.hpp"
#include "ohjelma.hpp"

kiitotie::kiitotie(std::string nimi, apuvalineet::piste alkupiste, double pituus, double suunta, double alkunousukorkeus, double alkunoususuunta) {
	std::clog << "kiitotie::kiitotie(" << nimi << " " << alkupiste.x << " " << alkupiste.y << " " << pituus << " " << suunta << " " << alkunousukorkeus << ")" << std::endl;

	this->alkupiste = alkupiste;
	this->pituus = pituus;
	this->suunta = suunta;

	this->alkunousukorkeus = alkunousukorkeus;
	this->alkunoususuunta = alkunoususuunta;
	this->nimi = nimi;

	this->loppupiste 		= apuvalineet::uusi_paikka(this->alkupiste, this->suunta, this->pituus);
	this->lahestymispiste 	= apuvalineet::uusi_paikka(this->alkupiste, this->suunta - 180.0, Asetukset::anna_asetus("lahestymispiste"));
	this->hidastuspiste 	= apuvalineet::uusi_paikka(this->alkupiste, this->suunta - 180.0, Asetukset::anna_asetus("hidastuspiste"));
	this->odotuspiste		= apuvalineet::uusi_paikka(this->alkupiste, this->suunta - 90.0, 0.15);
}
