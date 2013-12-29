#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lentokone.hpp"
#include "peli.hpp"
#include "pelicontroller.hpp"
#include "peliview.hpp"
#include "ohjelma.hpp"
#include "piirtopinta.hpp"

using ::testing::Return;
using ::testing::Sequence;

class MockOhjelma : public IOhjelma
{
public:
	MOCK_METHOD1(sekunnit, double(bool nollaa));
	MOCK_METHOD1(odota, void(unsigned int ms));
	MOCK_METHOD0(odota_nappi, nappi());
	MOCK_METHOD1(lue_nappi, bool(nappi n));
	MOCK_METHOD0(tyhjenna_syote, void());
	MOCK_METHOD3(onko_alueella, bool(const apuvalineet::piste& a, const apuvalineet::piste& b, double sade));
	MOCK_METHOD0(anna_hiiri, apuvalineet::piste());
	MOCK_METHOD0(lue_hiiri, bool());

	MOCK_METHOD0(lue_syote, std::string());
	MOCK_METHOD0(tyhjenna_viesti, void());
	MOCK_METHOD0(anna_viesti, std::string());
};

class MockPinta : public IPiirtoPinta
{
	MOCK_METHOD5(rectangleColor, void(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color));
	MOCK_METHOD5(lineColor, void(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color));
	MOCK_METHOD4(circleColor, void(unsigned short x1, unsigned short y1, unsigned short x2, unsigned int color));
	MOCK_METHOD7(trigonColor, void(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, unsigned int color));
	MOCK_METHOD4(piirra_kuva, void(const char *tiedosto, int x, int y, bool keskikohta));
	MOCK_METHOD4(kirjoita_tekstia, void(std::string teksti, int x, int y, bool aktiivinen));
	MOCK_METHOD0(get_fontinkoko, int());
	MOCK_METHOD0(flip, void());
};

TEST(LentokoneConstructor, LentokoneConstructor) {
	Asetukset asetukset;
	MockOhjelma ohjelmaMock;
    Kieli kieli;
    Peli peli(asetukset, ohjelmaMock, kieli, "EFKT.TXT");
	apuvalineet::piste paikka;
	paikka.x = 0;
	paikka.y = 0;
	lentokone kone(/*peli, ohjelmaMock, asetukset, */"", paikka, 0, 0, 0, 0, false);
}

TEST(PeliControllerTests, LongTimeInterval) {
	Asetukset asetukset;
	MockOhjelma ohjelmaMock;
	MockPinta pinta;
	Kieli kieli;
	Peli peli(asetukset, ohjelmaMock, kieli, "EFKT.TXT");
	PeliView peliview(pinta, asetukset, peli, ohjelmaMock, kieli);
	PeliController controller(peli, peliview, ohjelmaMock, asetukset, kieli);

	double timef = 0;

	ON_CALL(ohjelmaMock, sekunnit(true)).WillByDefault(Return(timef+=1.0));
	ON_CALL(ohjelmaMock, sekunnit(false)).WillByDefault(Return(timef += 1.0));

	// Currently calling aja() will run forever
	//controller.aja();
}

TEST(ValikkoTests, ValikkoBounds) {
	Asetukset asetukset;
	MockOhjelma ohjelmaMock;
	MockPinta pinta;
	Kieli kieli;
	ValikkoData valikkoData;
	ValikkoView valikkoView(pinta, valikkoData);
	valikko v(ohjelmaMock, valikkoView, valikkoData);

	IOhjelma::nappi nappisequence[] = {
		IOhjelma::nappi::NAPPI_YLOS,
		IOhjelma::nappi::NAPPI_YLOS,
		IOhjelma::nappi::NAPPI_YLOS,
		IOhjelma::nappi::NAPPI_ALAS,
		IOhjelma::nappi::NAPPI_ALAS,
		IOhjelma::nappi::NAPPI_ALAS,
		IOhjelma::nappi::NAPPI_ENTER,
	};

	Sequence seq;
	for (int i = 0; i < 7; i++)
	{
		EXPECT_CALL(ohjelmaMock, odota_nappi()).InSequence(seq).WillOnce(Return(nappisequence[i]));
	}

	int expected = 1;
	valikkoData.lisaa_kohta(0, "dummy1");
	valikkoData.lisaa_kohta(expected, "dummy1");

	int actual = v.aja();
	EXPECT_EQ(1, actual);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}
