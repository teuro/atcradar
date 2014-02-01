#include "lentokone.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

lentokone::lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus, lentokentta& k, Atis& a, IAsetukset& as) : kentta(k), atis(a), asetukset(as) {
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

    poistetaanko = false;

	odotuskuvio = -1;

	this->odotus = odotus;
	this->oikotie = false;

    this->nopeus_muutos = asetukset.anna_asetus("nopeus_muutos");
    this->korkeus_muutos = asetukset.anna_asetus("korkeus_muutos");
    this->suunta_muutos = asetukset.anna_asetus("suunta_muutos");
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
    this->reitti.push(piste);
}

void lentokone::aseta_navipiste(apuvalineet::piste paikka) {
	this->kohde.paikka = paikka;
}

int lentokone::kaarron_suunta(double suunta) {
	double rad_suunta = apuvalineet::deg2rad(suunta);
	double rad_kone = apuvalineet::deg2rad(this->suunta);
	double k = rad_kone - rad_suunta;

	const double PII = M_PI;

	while (k <= PII) {
		k += 2 * PII;
	}

	while (k > PII) {
		k -= 2 * PII;
	}

	return (k > 0) ? apuvalineet::VASEN : apuvalineet::OIKEA;
}

void lentokone::ota_selvitys(int tyyppi, bool lahesty) {
    if (tyyppi == apuvalineet::OIKOTIE) {
        this->aseta_navipiste(this->ulosmenopiste);
        this->oikotie = true;
    } else if (tyyppi == apuvalineet::LAHESTYMIS) {
        this->lahestymisselvitys = lahesty;

        if (this->lahestymisselvitys) {
            std::vector <kiitotie> :: iterator haku = std::find(kentta.kiitotiet.begin(), kentta.kiitotiet.end(), atis.anna_laskukiitotie());

            if (haku == kentta.kiitotiet.end()) {
                throw std::runtime_error("Baanaa ei ole");
            }

            this->baana = *haku;
            this->aseta_navipiste(baana.lahestymispiste);
        }
    } else if (tyyppi == apuvalineet::KESKEYTA) {
        this->lahestymisselvitys = false;
        this->muuta_selvityskorkeutta(baana.alkunousukorkeus);
        this->muuta_selvityssuuntaa(baana.alkunoususuunta);
        this->muuta_selvitysnopeutta(250);
    }
}

void lentokone::lahesty() {
    tarkista_suunta_kohteeseen();

    poista_reitti();

	if (apuvalineet::onko_alueella(paikka, baana.lahestymispiste, 0.01)) {
		muuta_selvitysnopeutta(160.0);
		aseta_navipiste(baana.hidastuspiste);
		muuta_selvityskorkeutta(kentta.korkeus);
	} else if (apuvalineet::onko_alueella(paikka, baana.hidastuspiste, 0.01)) {
		muuta_selvitysnopeutta(135.0);
		aseta_navipiste(baana.alkupiste);
	} else if (apuvalineet::onko_alueella(paikka, baana.alkupiste, 0.01)) {
		korkeus = kentta.korkeus;
		muuta_selvitysnopeutta(0.0);
		aseta_navipiste(baana.loppupiste);
	}
}

void lentokone::tarkista_suunta_kohteeseen() {
	if (this->reitti.size() || (this->kohde.paikka.x > 0 && this->kohde.paikka.y > 0)) {
        apuvalineet::vektori vektori_kohteeseen = apuvalineet::suunta_vektori(this->paikka, this->kohde.paikka);

		kaarto = kaarron_suunta(vektori_kohteeseen.suunta);
		muuta_selvityssuuntaa(vektori_kohteeseen.suunta, kaarto);
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
