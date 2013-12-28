#include "lentokone.hpp"

lentokone::lentokone(std::string kutsutunnus, apuvalineet::piste paikka, double korkeus, double nopeus, double suunta, int tyyppi, bool odotus) {
	this->kutsutunnus = kutsutunnus;

	this->paikka = paikka;

	this->kohde.paikka.x = 0;
	this->kohde.paikka.y = 0;
//	this->reaktioaika = apuvalineet::arvo_luku(3, 8);

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
		/*double koneen_korkeus = peli.kentta.korkeus + apuvalineet::mi2ft(etaisyys * std::tan(apuvalineet::deg2rad(this->baana.liukupolku)));

		if (this->korkeus > koneen_korkeus) {
			this->korkeus = koneen_korkeus;
		}*/
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
	paikka = apuvalineet::uusi_paikka(paikka, suunta, nopeus * (aika / 3600.0));
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

void lentokone::lahesty(kiitotie& baana) {
	lahestymisselvitys = true;

	if (apuvalineet::onko_alueella(paikka, this->baana.lahestymispiste, 0.01)) {
		muuta_selvitysnopeutta(160.0);
		aseta_navipiste(this->baana.hidastuspiste);
		// TODO: Fix these
		//muuta_selvityskorkeutta(kentta.korkeus);
	} else if (apuvalineet::onko_alueella(paikka, this->baana.hidastuspiste, 0.01)) {
		muuta_selvitysnopeutta(135.0);
		aseta_navipiste(this->baana.alkupiste);
	} else if (apuvalineet::onko_alueella(paikka, this->baana.alkupiste, 0.01)) {
		//korkeus = kentta.korkeus;
		muuta_selvitysnopeutta(0.0);
		aseta_navipiste(this->baana.loppupiste);
	}
}

void lentokone::tarkista_suunta_kohteeseen() {
	if (this->reitti.size()) {
		apuvalineet::vektori vektori_kohteeseen = apuvalineet::suunta_vektori(this->paikka, this->kohde.paikka);

		kaarto = kaarron_suunta(vektori_kohteeseen.suunta);
		muuta_selvityssuuntaa(vektori_kohteeseen.suunta, kaarto);
	}
}

void lentokone::ota_selvitys(double komento, int toiminto) {
	std::ofstream ulos("selvitykset.txt", std::ios::app);

	// TODO: Fix this
	double luku = 666;

	switch (toiminto) {
		case KORKEUS:
			muuta_selvityskorkeutta(luku);
			ulos << this->kutsutunnus << (selvityskorkeus > korkeus ? " nouse " : " laskeudu ") << luku << " jalkaan" << std::endl;
			break;
		case NOPEUS:
			muuta_selvitysnopeutta(luku);
			ulos << this->kutsutunnus << (selvitysnopeus > nopeus ? " kiihdytä " : " hidasta ") << luku << " solmuun" << std::endl;
			break;
	}
}

void lentokone::ota_selvitys(double suunta, int toiminto, int kaarto) {
    muuta_selvityssuuntaa(suunta, kaarto);
}

void lentokone::ota_selvitys(navipiste& piste) {
    aseta_navipiste(piste.paikka);
}

void lentokone::ota_selvitys(int toiminto) {
    switch (toiminto) {
    case LAHESTYMIS:
        this->lahestymisselvitys = true;
        break;
    case OIKOTIE:
        this->oikotie = true;
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
