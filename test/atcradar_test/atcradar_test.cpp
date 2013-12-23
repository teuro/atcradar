#include <gtest/gtest.h>

#include "lentokone.hpp"
#include "peli.hpp"
#include "ohjelma.hpp"

class OhjelmaMock : public IOhjelma
{
	virtual float sekunnit(bool nollaa = false) { return 0; }
	virtual void odota(double ms = 1) { }

	virtual nappi odota_nappi() { return nappi::NAPPI_MUU; }
	virtual bool lue_nappi(nappi n) { return false; }
	virtual void tyhjenna_syote() { }

	virtual bool onko_alueella(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade = 0.2) { return false; }
	virtual apuvalineet::piste anna_hiiri() { apuvalineet::piste tmp; tmp.x = tmp.y = 0; return tmp; }
	virtual bool lue_hiiri() { return false; }
};

TEST(LentokoneConstructor, LentokoneConstructor) {
	Asetukset asetukset;
	OhjelmaMock ohjelmaMock;
	Peli peli(ohjelmaMock, asetukset);
	apuvalineet::piste paikka;
	paikka.x = 0;
	paikka.y = 0;
	lentokone kone(peli, ohjelmaMock, asetukset, "", paikka, 0, 0, 0, 0, false);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}
