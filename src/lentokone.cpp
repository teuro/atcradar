#include "lentokone.hpp"
#include "peli.hpp"
#include "ohjelma.hpp"
#include <algorithm>

lentokone::lentokone() {}

lentokone::lentokone(std::string kutsutunnus, double x, double y, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus) {
	//std::clog << "Kone luotu " << kutsutunnus << " " << x << " " << y << " " << korkeus << " " << nopeus << " " << suunta << std::endl;
	this->kutsutunnus = kutsutunnus;

	this->paikka.x = x;
	this->paikka.y = y;

	this->kohde.paikka.x = 0;
	this->kohde.paikka.y = 0;

	this->laskubaana = -1;
	this->onko_porrastus = true;
	this->tyyppi = tyyppi;

	this->korkeus = korkeus;
	this->nopeus = nopeus;
	this->suunta = suunta;

	this->selvityskorkeus = this->korkeus;
	this->selvitysnopeus = this->nopeus;
	this->selvityssuunta = this->suunta;

	this->lahestymisselvitys = false;
	this->laskuselvitys = false;
	this->valittu = false;

	this->odotuskuvio = -1;

	this->odotus = odotus;

	std::clog << this->kutsutunnus << " luotu paikkaan " << this->paikka.x << ", " << this->paikka.y << std::endl;
}

lentokone::lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus) {
	//std::clog << "Kone luotu " << kutsutunnus << " " << " " << paikka.x << " " << paikka.y << " " << korkeus << " " << nopeus << " " << suunta << std::endl;
	this->kutsutunnus = kutsutunnus;
	this->paikka = paikka;

	this->kohde.paikka.x = 0;
	this->kohde.paikka.y = 0;

	this->laskubaana = -1;
	this->onko_porrastus = true;
	this->tyyppi = tyyppi;

	this->korkeus = korkeus;
	this->nopeus = nopeus;
	this->suunta = suunta;

	this->selvityskorkeus = this->korkeus;
	this->selvitysnopeus = this->nopeus;
	this->selvityssuunta = this->suunta;

	this->lahestymisselvitys = false;
	this->laskuselvitys = false;
	this->valittu = false;

	this->odotus = odotus;
	this->odotuskuvio = -1;

	std::clog << this->kutsutunnus << " luotu paikkaan " << this->paikka.x << ", " << this->paikka.y << std::endl;
}

void lentokone::muuta_selvityskorkeutta(double korkeus) {
	if (!this->lahestymisselvitys) {
		if (korkeus > ohjelma::anna_asetus("selvityskorkeus_yla")) {
			peli::aseta_virhe(peli::VIRHE_KORKEUS_YLA);
			return;
		} else if (korkeus < ohjelma::anna_asetus("selvityskorkeus_ala")) {
			peli::aseta_virhe(peli::VIRHE_KORKEUS_ALA);
			return;
		}
	}

	this->selvityskorkeus = korkeus;
}

void lentokone::muuta_selvitysnopeutta(double nopeus) {
	if (!this->lahestymisselvitys) {
		if (this->korkeus < ohjelma::anna_asetus("nopeusrajoituskorkeus")) {
			if (nopeus > ohjelma::anna_asetus("selvitysnopeus_yla")) {
				peli::aseta_virhe(peli::VIRHE_NOPEUS_YLA);
				return;
			} else if (nopeus < ohjelma::anna_asetus("selvitysnopeus_ala")) {
				peli::aseta_virhe(peli::VIRHE_NOPEUS_ALA);
				return;
			}
		}
	}

	this->selvitysnopeus = nopeus;
}

void lentokone::muuta_selvityssuuntaa(double _suunta, int kaarto) {
	if (_suunta > 360) {
		_suunta -= 360;
	} else if (_suunta < 0) {
		_suunta += 360;
	}
	this->selvityssuunta = _suunta;
	this->kaarto = kaarto;
}

void lentokone::muuta_selvityksia(double korkeus, double nopeus, double suunta, int kaarto) {
	this->muuta_selvityskorkeutta(korkeus);
	this->muuta_selvitysnopeutta(nopeus);
	this->muuta_selvityssuuntaa(suunta, kaarto);
}

void lentokone::muuta_tilaa(double aika) {
	//std::clog << "lentokone::muuta_tilaa(" << aika << ")" << std::endl;
	if (this->korkeus != this->selvityskorkeus) {
		this->muuta_korkeutta(aika);
	}
	if (this->nopeus != this->selvitysnopeus) {
		this->muuta_nopeutta(aika);
	}

	if (this->suunta != this->selvityssuunta) {
		this->muuta_suuntaa(aika);
	}

	//std::clog << this->korkeus << " " << this->nopeus << " " << this->suunta << std::endl;
}

void lentokone::muuta_korkeutta(double aika) {
	if (this->lahestymisselvitys == false) {
		if (std::abs(this->korkeus - this->selvityskorkeus) > 5) {
			this->korkeus += ((this->selvityskorkeus > this->korkeus) ? 1 : -1) * aika * ohjelma::anna_asetus("korkeus_muutos");
		} else {
			this->korkeus = this->selvityskorkeus;
		}
	} else {
		double etaisyys = apuvalineet::etaisyys(this->paikka, peli::kentta.kiitotiet[this->laskubaana].alkupiste);

		double koneen_korkeus = peli::kentta.korkeus + apuvalineet::mi2ft(etaisyys * std::tan(apuvalineet::deg2rad(ohjelma::anna_asetus("liukupolku"))));

		if (this->korkeus > koneen_korkeus) {
			this->korkeus = koneen_korkeus;
		}
	}
}

void lentokone::muuta_nopeutta(double aika) {
	if (std::abs(this->nopeus - this->selvitysnopeus) > 3) {
		this->nopeus += ((this->selvitysnopeus > this->nopeus) ? 1 : -1) * aika * ohjelma::anna_asetus("nopeus_muutos");
	} else {
		this->nopeus = this->selvitysnopeus;
	}
}

void lentokone::muuta_suuntaa(double aika) {
	if (this->suunta < 0) {
		suunta += 360.0;
	} else if (this->suunta > 360.0) {
		suunta -= 360.0;
	}
	if (std::abs(this->suunta - this->selvityssuunta) > 0.5) {
		if (this->lahestymisselvitys) {
			this->suunta += this->kaarto * aika * ohjelma::anna_asetus("lahestymissuunta_muutos");
		} else {
			this->suunta += this->kaarto * aika * ohjelma::anna_asetus("suunta_muutos");
		}
		//std::clog << this->kaarto << " * " << aika << " * " << ohjelma::anna_asetus("suunta_muutos") << " = " << this->suunta << std::endl;
	} else {
		this->suunta = this->selvityssuunta;
	}
}

void lentokone::liiku(double aika) {
	this->paikka = apuvalineet::uusi_paikka(this->paikka, this->suunta, this->nopeus * (aika / 3600.0));
	this->polttoaine -= ohjelma::anna_asetus("polttoaineen_kulutus") * (aika / 3600.0);
}

void lentokone::aseta_navipiste(navipiste paikka) {
	this->kohde = paikka;
}

void lentokone::aseta_navipiste(apuvalineet::piste paikka) {
	this->kohde.paikka = paikka;
}

int lentokone::kaarron_suunta(double suunta) {
	double rad_suunta = apuvalineet::deg2rad(suunta);
	double rad_kone = apuvalineet::deg2rad(this->suunta);
	double k = rad_kone - rad_suunta;
	double PII = 3.14;
	//std::clog << rad_kone << " / " << rad_suunta << std::endl;

	while (k <= PII) {
		k += 2 * PII;
	}

	while (k > PII) {
		k -= 2 * PII;
	}

	return (k > 0) ? VASEN : OIKEA;
}

void lentokone::lahesty() {
	this->lahestymisselvitys = true;

	if (ohjelma::onko_alueella(this->paikka, peli::kentta.kiitotiet[this->laskubaana].lahestymispiste, 0.01)) {
		this->muuta_selvitysnopeutta(ohjelma::anna_asetus("alkulahestymisnopeus"));
		this->aseta_navipiste(peli::kentta.kiitotiet[this->laskubaana].hidastuspiste);
		this->muuta_selvityskorkeutta(peli::kentta.korkeus);
	} else if (ohjelma::onko_alueella(this->paikka, peli::kentta.kiitotiet[this->laskubaana].hidastuspiste, 0.01)) {
		this->muuta_selvitysnopeutta(ohjelma::anna_asetus("loppulahestymisnopeus"));
		this->aseta_navipiste(peli::kentta.kiitotiet[this->laskubaana].alkupiste);
	} else if (ohjelma::onko_alueella(this->paikka, peli::kentta.kiitotiet[this->laskubaana].alkupiste, 0.01)) {
		this->korkeus = peli::kentta.korkeus;
		this->muuta_selvitysnopeutta(0.0);
		this->aseta_navipiste(peli::kentta.kiitotiet[this->laskubaana].loppupiste);
	}
}

void lentokone::tarkista_suunta_kohteeseen() {
	if (this->kohde.paikka.x > 0 && this->kohde.paikka.y > 0) {
		apuvalineet::vektori vektori_kohteeseen = apuvalineet::suunta_vektori(this->paikka, this->kohde.paikka);

		this->kaarto = this->kaarron_suunta(vektori_kohteeseen.suunta);
		this->muuta_selvityssuuntaa(vektori_kohteeseen.suunta, this->kaarto);
	}
}

void lentokone::ota_selvitys(std::string tmp, int toiminto) {
	if (toiminto == peli::SUUNTA) {
		std::vector <navipiste>::iterator etsi = std::find(peli::sisapisteet.begin(), peli::sisapisteet.end(), tmp);

		if (etsi != peli::sisapisteet.end()) {
			this->reitti.push(*etsi);
			tmp = "";
		}

		if (this->kohde.paikka.x == 0 && this->kohde.paikka.y == 0 && this->reitti.size()) {
			this->kohde = anna_piste();
		}

		if (tmp.substr(0, 1) == "V" || tmp.substr(0, 1) == "v") {
			kaarto = VASEN;
			tmp = tmp.substr(1, std::string::npos);
		} else if (tmp.substr(0, 1) == "O" || tmp.substr(0, 1) == "o") {
			kaarto = OIKEA;
			tmp = tmp.substr(1, std::string::npos);
		} else {
			kaarto = VASEN;
		}
	}

	double luku = apuvalineet::luvuksi<double>(tmp);

	switch (toiminto) {
		case peli::SUUNTA:
			this->muuta_selvityssuuntaa(luku, kaarto);
			break;
		case peli::KORKEUS:
			this->muuta_selvityskorkeutta(luku);
			break;
		case peli::NOPEUS:
			this->muuta_selvitysnopeutta(luku);
			break;
        case peli::ODOTUS:
            this->odotuskuvio = ohjelma::sekunnit() + 60;
            break;
        case peli::POIS:
            this->odotuskuvio = -1;
            break;
		case peli::OIKOTIE:
			if (this->korkeus < ohjelma::anna_asetus("oikotie")) {
				peli::aseta_virhe(peli::VIRHE_OIKOTIE);
			} else {
				this->aseta_navipiste(this->ulosmenopiste.paikka);
			}
			break;
		case peli::LAHESTYMIS:
			this->poista_reitti();
			double koneen_suunta = this->suunta;

			double min_suunta = peli::kentta.kiitotiet[peli::atis::laskukiitotie].suunta - ohjelma::anna_asetus("lahestymiskulma");
			double max_suunta = peli::kentta.kiitotiet[peli::atis::laskukiitotie].suunta + ohjelma::anna_asetus("lahestymiskulma");

			if (this->korkeus > ohjelma::anna_asetus("maks_lahestymiskorkeus")) {
				aseta_virhe(peli::VIRHE_LAHESTYMISKORKEUS);
			} else if (this->nopeus > ohjelma::anna_asetus("maks_lahestymisnopeus")) {
				aseta_virhe(peli::VIRHE_LAHESTYMISNOPEUS);
			} else if (koneen_suunta < min_suunta || koneen_suunta > max_suunta) {
				aseta_virhe(peli::VIRHE_LAHESTYMISSUUNTA);
			} else {
				this->aseta_navipiste(peli::kentta.kiitotiet[peli::atis::laskukiitotie].lahestymispiste);
				this->laskubaana = peli::atis::laskukiitotie;
			}
	}
}

void lentokone::poista_reitti() {
	while (this->reitti.size()) {
		this->reitti.pop();
	}
}

navipiste lentokone::anna_piste() {
	navipiste tmp = this->reitti.front();

	return tmp;
}
