#include "lentokone.hpp"
#include "peli.hpp"
#include "ohjelma.hpp"

lentokone::lentokone() {}

lentokone::lentokone(std::string kutsutunnus, double x, double y, double korkeus, double nopeus, double suunta, int tyyppi) {
	//std::clog << "Kone luotu " << kutsutunnus << " " << x << " " << y << " " << korkeus << " " << nopeus << " " << suunta << std::endl;
	this->kutsutunnus = kutsutunnus;

	this->paikka.x = x;
	this->paikka.y = y;

	this->kohde.x = 0;
	this->kohde.y = 0;

	this->laskubaana = -1;
	this->onko_porrastus = true;
	this->ulkona = false;
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

	std::clog << this->kutsutunnus << " luotu paikkaan " << this->paikka.x << ", " << this->paikka.y << std::endl;
}

lentokone::lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi) {
	//std::clog << "Kone luotu " << kutsutunnus << " " << " " << paikka.x << " " << paikka.y << " " << korkeus << " " << nopeus << " " << suunta << std::endl;
	this->kutsutunnus = kutsutunnus;
	this->paikka = paikka;

	this->kohde.x = 0;
	this->kohde.y = 0;

	this->laskubaana = -1;
	this->onko_porrastus = true;
	this->ulkona = false;
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
		if (nopeus > ohjelma::anna_asetus("selvitysnopeus_yla")) {
			peli::aseta_virhe(peli::VIRHE_NOPEUS_YLA);
			return;
		} else if (nopeus < ohjelma::anna_asetus("selvitysnopeus_ala")) {
			peli::aseta_virhe(peli::VIRHE_NOPEUS_ALA);
			return;
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
	//std::clog << "lentokone::liiku(" << aika << ")" << std::endl;
	double x = this->paikka.x + apuvalineet::nm2px(this->nopeus * (aika / 3600.0)) * cos(apuvalineet::deg2rad(this->suunta - 90));
	double y = this->paikka.y + apuvalineet::nm2px(this->nopeus * (aika / 3600.0)) * sin(apuvalineet::deg2rad(this->suunta - 90));

	this->paikka.x = x;
	this->paikka.y = y;

	if (this->paikka.x > ohjelma::anna_asetus("ruutu_leveys") || this->paikka.x < 0 || this->paikka.y < 0 || this->paikka.y > ohjelma::anna_asetus("ruutu_korkeus")) {
		this->ulkona = true;
	}
}

void lentokone::aseta_navipiste(apuvalineet::piste paikka) {
	this->kohde = paikka;
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
	apuvalineet::vektori vektori_kohteeseen = apuvalineet::suunta_vektori(this->paikka, this->kohde);

	this->kaarto = this->kaarron_suunta(vektori_kohteeseen.suunta);
	this->muuta_selvityssuuntaa(vektori_kohteeseen.suunta, this->kaarto);
}

void lentokone::ota_selvitys(std::string tmp, int toiminto) {
	if (toiminto == peli::SUUNTA) {
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
	}
}
