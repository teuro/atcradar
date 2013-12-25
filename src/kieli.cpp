#include "kieli.hpp"

Kieli::Kieli(std::string kieli) {
	std::clog << "Luodaan uusi kieli " << kieli << std::endl;
	this->kieli = kieli;
	int kohta = -1;
	
	if (kieli == "fi_FI") {
		kohta = 1;
	} else if (kieli == "en_EN") {
		kohta = 2;
	}
	
	std::ifstream sisaan("data/tekstit.txt", std::ios::in);
	size_t pos = 0;
	
	std::string sana;
	std::string erotin = "|";
	std::string rivi;
	std::vector <std::string> asiat;
	
	while (std::getline(sisaan, rivi)) {
		while ((pos = rivi.find(erotin)) != std::string::npos) {
			sana = rivi.substr(0, pos);
			asiat.push_back(sana);
			rivi.erase(0, pos + erotin.length());
		}
		
		this->lisaa_teksti(apuvalineet::luvuksi<int>(asiat[0]), asiat[kohta]);
		asiat.clear();
	}
}

void Kieli::lisaa_teksti(int id, std::string teksti) {
	//std::clog << "Kieli::lisaa_teksti(" << id << ", " << teksti << ")" << std::endl;
	this->tekstit[id] = teksti;
}

std::string Kieli::anna_teksti(int id) {
	std::clog << "Kieli::anna_teksti(" << id << ") = " << this->tekstit[id] <<  std::endl;
	
	return this->tekstit[id];
}