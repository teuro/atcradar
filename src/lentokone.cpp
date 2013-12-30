#include "lentokone.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

lentokone::lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus) {
	this->kutsutunnus = kutsutunnus;

	this->paikka = paikka;

	this->kohde.paikka.x = 0;
	this->kohde.paikka.y = 0;

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
	this->poistetaan = false;

	this->odotus = odotus;
	this->oikotie = false;

	this->nopeus_muutos = 4.0;
	this->korkeus_muutos = 35.0;
	this->suunta_muutos = 3.0;
}

void lentokone::muuta_selvityskorkeutta(double korkeus) {
	selvityskorkeus = korkeus;
}

void lentokone::muuta_selvitysnopeutta(double nopeus) {
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

void lentokone::muuta_tilaa(double aika) {
	if (korkeus != selvityskorkeus) {
		muuta_korkeutta(aika);
	}
	if (nopeus != selvitysnopeus) {
		muuta_nopeutta(aika);
	}

	if (suunta != selvityssuunta) {
		muuta_suuntaa(aika);
	}
}

void lentokone::muuta_korkeutta(double aika) {
	if (lahestymisselvitys == false) {
		if (std::abs(korkeus - selvityskorkeus) > 5) {
			korkeus += ((selvityskorkeus > korkeus) ? 1 : -1) * aika * this->korkeus_muutos;
		} else {
			korkeus = selvityskorkeus;
		}
	} else {
		double etaisyys = apuvalineet::etaisyys(paikka, baana.alkupiste);

		// TODO: Fix this
		double koneen_korkeus = this->kentta.korkeus + apuvalineet::mi2ft(etaisyys * std::tan(apuvalineet::deg2rad(this->baana.liukupolku)));

		if (this->korkeus > koneen_korkeus) {
			this->korkeus = koneen_korkeus;
		}
	}
}

void lentokone::muuta_nopeutta(double aika) {
	if (std::abs(nopeus - selvitysnopeus) > 3) {
		nopeus += ((selvitysnopeus > nopeus) ? 1 : -1) * aika * this->nopeus_muutos;
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
        suunta += kaarto * aika * this->suunta_muutos;
    } else {
		suunta = selvityssuunta;
	}
}

void lentokone::liiku(double aika) {
    this->muuta_tilaa(aika);

    if (this->reitti.size()) {
        if (this->kohde.nimi == this->anna_piste().nimi) {
            this->tarkista_suunta_kohteeseen();
        } else {
            this->kohde = this->anna_piste();
        }
    } else if (this->lahestymisselvitys) {
        this->lahesty();
    }

	paikka = apuvalineet::uusi_paikka(paikka, suunta, nopeus * (aika / 3600.0));
}

void lentokone::aseta_navipiste(navipiste piste) {
	this->kohde = piste;
}

void lentokone::aseta_navipiste(apuvalineet::piste paikka) {
	this->kohde.paikka = paikka;
}

int lentokone::kaarron_suunta(double suunta) {
	double rad_suunta = apuvalineet::deg2rad(suunta);
	double rad_kone = apuvalineet::deg2rad(this->suunta);
	double k = rad_kone - rad_suunta;

	const double PII = M_PI;
	//std::clog << rad_kone << " / " << rad_suunta << std::endl;

	while (k <= PII) {
		k += 2 * PII;
	}

	while (k > PII) {
		k -= 2 * PII;
	}

	return (k > 0) ? apuvalineet::VASEN : apuvalineet::OIKEA;
}

void lentokone::lahesty() {
    this->tarkista_suunta_kohteeseen();

    this->poista_reitti();

	if (apuvalineet::onko_alueella(paikka, this->baana.lahestymispiste, 0.01)) {
		this->muuta_selvitysnopeutta(160.0);
		this->aseta_navipiste(this->baana.hidastuspiste);
		this->muuta_selvityskorkeutta(this->kentta.korkeus);
	} else if (apuvalineet::onko_alueella(paikka, this->baana.hidastuspiste, 0.01)) {
		this->muuta_selvitysnopeutta(135.0);
		this->aseta_navipiste(this->baana.alkupiste);
	} else if (apuvalineet::onko_alueella(paikka, this->baana.alkupiste, 0.01)) {
		this->korkeus = this->kentta.korkeus;
		this->muuta_selvitysnopeutta(0.0);
		this->aseta_navipiste(this->baana.loppupiste);
	}
}

void lentokone::tarkista_suunta_kohteeseen() {
	if (this->reitti.size() || (this->kohde.paikka.x > 0 && this->kohde.paikka.y > 0)) {
        apuvalineet::vektori vektori_kohteeseen = apuvalineet::suunta_vektori(this->paikka, this->kohde.paikka);

		kaarto = kaarron_suunta(vektori_kohteeseen.suunta);
		muuta_selvityssuuntaa(vektori_kohteeseen.suunta, kaarto);
	}
}

void lentokone::ota_selvitys(double komento, int toiminto) {
	switch (toiminto) {
		case apuvalineet::KORKEUS:
			muuta_selvityskorkeutta(komento);
			break;
		case apuvalineet::NOPEUS:
			muuta_selvitysnopeutta(komento);
			break;
	}
}

void lentokone::ota_selvitys(double suunta, int toiminto, int kaarto) {
	this->muuta_selvityssuuntaa(suunta, kaarto);
}

void lentokone::ota_selvitys(navipiste& piste) {
	this->reitti.push(piste);
}

void lentokone::ota_selvitys(int toiminto, kiitotie& baana, lentokentta& kentta) {
    this->lahestymisselvitys = true;
    this->baana = baana;
    this->kentta = kentta;
    this->aseta_navipiste(baana.lahestymispiste);
}

void lentokone::ota_selvitys(int toiminto) {
    this->oikotie = true;
    this->reitti.push(this->ulosmenopiste);
}

void lentokone::poista_reitti() {
	while (this->reitti.size()) {
		this->reitti.pop();
	}
}

navipiste lentokone::anna_piste() {
	return reitti.front();
}
