#include "kieli.hpp"

Kieli::Kieli(std::string kieli) {
	std::clog << "Luodaan uusi kieli " << kieli << std::endl;
	this->kieli = kieli;
	int kohta = -1;
	
	if (kieli == "fi_FI") {
		kohta = 1;
	} else if (kieli == "en_EN") {
		kohta = 2;
	} else {
		throw std::logic_error("Pyydettyä kieltä " + kieli + "ei ole vielä tehty");
	}
	
	std::ifstream sisaan("data/tekstit.txt", std::ios::in);
	
	std::string rivi;
	std::vector <std::string> asiat;
	
	while (std::getline(sisaan, rivi)) {
		asiat = apuvalineet::pilko_rivi(rivi, "|");
		
		this->lisaa_teksti(apuvalineet::luvuksi<int>(asiat[0]), asiat[kohta]);
		asiat.clear();
	}
}

void Kieli::lisaa_teksti(int id, std::string teksti) {
	this->tekstit[id] = teksti;
}

std::string Kieli::anna_teksti(int id) {
	
	return this->tekstit[id];
}