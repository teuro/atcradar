#include "asetukset.h"
#include "lentokone.hpp"
#include "peli.hpp"
#include "ohjelma.hpp"
#include <algorithm>

/*lentokone::lentokone(const lentokone& lk) : ohjelma(lk.ohjelma), peli(lk.peli)
{

}*/

lentokone::lentokone(Peli& p, Ohjelma& o, std::string kutsutunnus, double x, double y, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus) : peli(p), ohjelma(o) {
	//std::clog << "Kone luotu " << kutsutunnus << " " << x << " " << y << " " << korkeus << " " << nopeus << " " << suunta << std::endl;
	this->kutsutunnus = kutsutunnus;

	paikka.x = x;
	paikka.y = y;

	this->kohde.paikka.x = 0;
	this->kohde.paikka.y = 0;
	this->reaktioaika = apuvalineet::arvo_luku(3, 8);

	laskubaana = -1;
	onko_porrastus = true;
	this->tyyppi = tyyppi;

	this->korkeus = korkeus;
	this->nopeus = nopeus;
	this->suunta = suunta;

	selvityskorkeus = korkeus;
	selvitysnopeus = nopeus;
	selvityssuunta = suunta;

	lahestymisselvitys = false;
	laskuselvitys = false;
	valittu = false;

	odotuskuvio = -1;

	this->odotus = odotus;

	//std::clog << kutsutunnus << " luotu paikkaan " << paikka.x << ", " << paikka.y << std::endl;
}

lentokone::lentokone(Peli& p, Ohjelma& o, std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus) : peli(p), ohjelma(o) {
    //std::clog << "Kone luotu " << kutsutunnus << " " << " " << paikka.x << " " << paikka.y << " " << korkeus << " " << nopeus << " " << suunta << std::endl;
	this->kutsutunnus = kutsutunnus;
	this->paikka = paikka;
	this->reaktioaika = apuvalineet::arvo_luku(3, 8);

	this->kohde.paikka.x = 0;
	this->kohde.paikka.y = 0;

	laskubaana = -1;
	onko_porrastus = true;
	this->tyyppi = tyyppi;

	this->korkeus = korkeus;
	this->nopeus = nopeus;
	this->suunta = suunta;

	selvityskorkeus = korkeus;
	selvitysnopeus = nopeus;
	selvityssuunta = suunta;

	lahestymisselvitys = false;
	laskuselvitys = false;
	valittu = false;

	this->odotus = odotus;
	odotuskuvio = -1;

	std::clog << kutsutunnus << " luotu paikkaan " << paikka.x << ", " << paikka.y << std::endl;
}

void lentokone::muuta_selvityskorkeutta(double korkeus) {
	if (!lahestymisselvitys) {
		if (korkeus > Asetukset::anna_asetus("selvityskorkeus_yla")) {
			peli.aseta_virhe(Peli::VIRHE_KORKEUS_YLA);
			return;
        }
        else if (korkeus < Asetukset::anna_asetus("selvityskorkeus_ala")) {
			peli.aseta_virhe(Peli::VIRHE_KORKEUS_ALA);
			return;
		}
	}

	selvityskorkeus = korkeus;
}

void lentokone::muuta_selvitysnopeutta(double nopeus) {
	if (!lahestymisselvitys) {
		if (korkeus < Asetukset::anna_asetus("nopeusrajoituskorkeus")) {
			if (nopeus > Asetukset::anna_asetus("selvitysnopeus_yla")) {
				peli.aseta_virhe(Peli::VIRHE_NOPEUS_YLA);
				return;
			} else if (nopeus < Asetukset::anna_asetus("selvitysnopeus_ala")) {
				peli.aseta_virhe(Peli::VIRHE_NOPEUS_ALA);
				return;
			}
		}
	}

	selvitysnopeus = nopeus;
}

void lentokone::muuta_selvityssuuntaa(double _suunta, int kaarto) {
	if (_suunta > 360) {
		_suunta -= 360;
	} else if (_suunta < 0) {
		_suunta += 360;
	}
	selvityssuunta = _suunta;
	this->kaarto = kaarto;
}

void lentokone::muuta_selvityksia(double korkeus, double nopeus, double suunta, int kaarto) {
	muuta_selvityskorkeutta(korkeus);
	muuta_selvitysnopeutta(nopeus);
	muuta_selvityssuuntaa(suunta, kaarto);
}

void lentokone::muuta_tilaa(double aika) {
	//std::clog << "lentokone::muuta_tilaa(" << aika << ")" << std::endl;
	if (korkeus != selvityskorkeus) {
		muuta_korkeutta(aika);
	}
	if (nopeus != selvitysnopeus) {
		muuta_nopeutta(aika);
	}

	if (suunta != selvityssuunta) {
		muuta_suuntaa(aika);
	}

	//std::clog << korkeus << " " << nopeus << " " << suunta << std::endl;
}

void lentokone::muuta_korkeutta(double aika) {
	if (lahestymisselvitys == false) {
		if (std::abs(korkeus - selvityskorkeus) > 5) {
			korkeus += ((selvityskorkeus > korkeus) ? 1 : -1) * aika * Asetukset::anna_asetus("korkeus_muutos");
		} else {
			korkeus = selvityskorkeus;
		}
	} else {
		double etaisyys = apuvalineet::etaisyys(paikka, peli.kentta.kiitotiet[laskubaana].alkupiste);

		double koneen_korkeus = peli.kentta.korkeus + apuvalineet::mi2ft(etaisyys * std::tan(apuvalineet::deg2rad(Asetukset::anna_asetus("liukupolku"))));

		if (korkeus > koneen_korkeus) {
			korkeus = koneen_korkeus;
		}
	}
}

void lentokone::muuta_nopeutta(double aika) {
	if (std::abs(nopeus - selvitysnopeus) > 3) {
		nopeus += ((selvitysnopeus > nopeus) ? 1 : -1) * aika * Asetukset::anna_asetus("nopeus_muutos");
	} else {
		nopeus = selvitysnopeus;
	}
}

void lentokone::muuta_suuntaa(double aika) {
	if (suunta < 0) {
		suunta += 360.0;
	} else if (suunta > 360.0) {
		suunta -= 360.0;
	}
	if (std::abs(suunta - selvityssuunta) > 0.5) {
		if (lahestymisselvitys) {
			suunta += kaarto * aika * Asetukset::anna_asetus("lahestymissuunta_muutos");
		} else {
			suunta += kaarto * aika * Asetukset::anna_asetus("suunta_muutos");
		}
		//std::clog << kaarto << " * " << aika << " * " << Asetukset::anna_asetus("suunta_muutos") << " = " << suunta << std::endl;
	} else {
		suunta = selvityssuunta;
	}
}

void lentokone::liiku(double aika) {
	paikka = apuvalineet::uusi_paikka(paikka, suunta, nopeus * (aika / 3600.0));
	polttoaine -= Asetukset::anna_asetus("polttoaineen_kulutus") * (aika / 3600.0);
}

void lentokone::aseta_navipiste(navipiste paikka) {
	this->kohde = paikka;
}

void lentokone::aseta_navipiste(apuvalineet::piste paikka) {
	this->kohde.paikka = paikka;
}

int lentokone::kaarron_suunta(double suunta) {
	double rad_suunta = apuvalineet::deg2rad(suunta);
	double rad_kone = apuvalineet::deg2rad(suunta);
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
	lahestymisselvitys = true;

	if (ohjelma.onko_alueella(paikka, peli.kentta.kiitotiet[laskubaana].lahestymispiste, 0.01)) {
		muuta_selvitysnopeutta(Asetukset::anna_asetus("alkulahestymisnopeus"));
		aseta_navipiste(peli.kentta.kiitotiet[laskubaana].hidastuspiste);
		muuta_selvityskorkeutta(peli.kentta.korkeus);
	} else if (ohjelma.onko_alueella(paikka, peli.kentta.kiitotiet[laskubaana].hidastuspiste, 0.01)) {
		muuta_selvitysnopeutta(Asetukset::anna_asetus("loppulahestymisnopeus"));
		aseta_navipiste(peli.kentta.kiitotiet[laskubaana].alkupiste);
	} else if (ohjelma.onko_alueella(paikka, peli.kentta.kiitotiet[laskubaana].alkupiste, 0.01)) {
		korkeus = peli.kentta.korkeus;
		muuta_selvitysnopeutta(0.0);
		aseta_navipiste(peli.kentta.kiitotiet[laskubaana].loppupiste);
	}
}

void lentokone::tarkista_suunta_kohteeseen() {
	if (this->kohde.paikka.x > 0 && this->kohde.paikka.y > 0) {
		apuvalineet::vektori vektori_kohteeseen = apuvalineet::suunta_vektori(this->paikka, this->kohde.paikka);

		kaarto = kaarron_suunta(vektori_kohteeseen.suunta);
		muuta_selvityssuuntaa(vektori_kohteeseen.suunta, kaarto);
	}
}

void lentokone::ota_selvitys(std::string tmp, int toiminto) {
	// TODO: Input parsing (tmp) should not be in this class. Move it to Pelicontroller
	if (toiminto == Peli::SUUNTA) {
        std::vector <navipiste>::iterator etsi = std::find(peli.sisapisteet.begin(), peli.sisapisteet.end(), tmp);

        if (etsi != peli.sisapisteet.end()) {
			reitti.push(*etsi);
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
		case Peli::SUUNTA:
			muuta_selvityssuuntaa(luku, kaarto);
			break;
		case Peli::KORKEUS:
			muuta_selvityskorkeutta(luku);
			break;
		case Peli::NOPEUS:
			muuta_selvitysnopeutta(luku);
			break;
        case Peli::ODOTUS:
            odotuskuvio = ohjelma.sekunnit() + 60;
            break;
        case Peli::POIS:
            odotuskuvio = -1;
            break;
		case Peli::OIKOTIE:
			if (korkeus < Asetukset::anna_asetus("oikotie")) {
				peli.aseta_virhe(Peli::VIRHE_OIKOTIE);
			} else {
				aseta_navipiste(ulosmenopiste.paikka);
			}
			break;
		case Peli::LAHESTYMIS:
			this->poista_reitti();
			double koneen_suunta = this->suunta;

            double min_suunta = peli.kentta.kiitotiet[peli.atis.laskukiitotie].suunta - Asetukset::anna_asetus("lahestymiskulma");
            double max_suunta = peli.kentta.kiitotiet[peli.atis.laskukiitotie].suunta + Asetukset::anna_asetus("lahestymiskulma");

			if (korkeus > Asetukset::anna_asetus("maks_lahestymiskorkeus")) {
				peli.aseta_virhe(Peli::VIRHE_LAHESTYMISKORKEUS);
			} else if (nopeus > Asetukset::anna_asetus("maks_lahestymisnopeus")) {
				peli.aseta_virhe(Peli::VIRHE_LAHESTYMISNOPEUS);
			} else if (koneen_suunta < min_suunta || koneen_suunta > max_suunta) {
				peli.aseta_virhe(Peli::VIRHE_LAHESTYMISSUUNTA);
			} else {
				aseta_navipiste(peli.kentta.kiitotiet[peli.atis.laskukiitotie].lahestymispiste);
				laskubaana = peli.atis.laskukiitotie;
			}
	}
}

void lentokone::poista_reitti() {
	while (this->reitti.size()) {
		this->reitti.pop();
	}
}

navipiste lentokone::anna_piste() {
	return reitti.front();
}
