#include "lentokentta.hpp"
#include "ohjelma.hpp"

apuvalineet::piste kiitotie::laske_loppupiste() {
	int x = this->alkupiste.x + apuvalineet::nm2px(this->pituus) * cos(apuvalineet::deg2rad(this->suunta - 90));
	int y = this->alkupiste.y + apuvalineet::nm2px(this->pituus) * sin(apuvalineet::deg2rad(this->suunta - 90));

	this->loppupiste.x = x;
	this->loppupiste.y = y;

	return this->loppupiste;
}

apuvalineet::piste kiitotie::laske_hidastuspiste() {
	int x = this->alkupiste.x - apuvalineet::nm2px(ohjelma::anna_asetus("hidastuspiste")) * cos(apuvalineet::deg2rad(this->suunta - 90));
	int y = this->alkupiste.y - apuvalineet::nm2px(ohjelma::anna_asetus("hidastuspiste")) * sin(apuvalineet::deg2rad(this->suunta - 90));

	this->hidastuspiste.x = x;
	this->hidastuspiste.y = y;

	return this->hidastuspiste;
}

apuvalineet::piste kiitotie::laske_lahestymispiste() {
	int x = this->alkupiste.x - apuvalineet::nm2px(ohjelma::anna_asetus("lahestymispiste")) * cos(apuvalineet::deg2rad(this->suunta - 90));
	int y = this->alkupiste.y - apuvalineet::nm2px(ohjelma::anna_asetus("lahestymispiste")) * sin(apuvalineet::deg2rad(this->suunta - 90));

	this->lahestymispiste.x = x;
	this->lahestymispiste.y = y;

	return this->lahestymispiste;
}

kiitotie::kiitotie(std::string nimi, apuvalineet::piste alkupiste, double pituus, double suunta, double alkunousukorkeus, double alkunoususuunta) {
	std::clog << "kiitotie::kiitotie(" << alkupiste.x << " " << alkupiste.y << " " << pituus << " " << suunta << " " << alkunousukorkeus << ")" << std::endl;
	this->alkupiste = alkupiste;
	this->pituus = pituus;
	this->suunta = suunta;
	this->valittu = false;
	this->alkunousukorkeus = alkunousukorkeus;
	this->alkunoususuunta = alkunoususuunta;
	this->nimi = nimi;

	this->loppupiste = laske_loppupiste();
	this->lahestymispiste = laske_lahestymispiste();
	this->hidastuspiste = laske_hidastuspiste();
}
