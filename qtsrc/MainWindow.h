#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

#include "levelmenu.h"
#include "atisview.h"
//#include "peliview.h"
#include "PeliWidget.h"

#include "../../src/peliview.hpp"
#include "../../src/peli.hpp"
#include "../../src/pelicontroller.hpp"

class MainWindow : public QDialog, public IOhjelma
{
	Q_OBJECT

public:
	MainWindow()
	{
		levelMenu = new LevelMenu();
		atisView = new AtisView();
		//peliView = new PeliView();

        Peli *peli = new Peli(asetukset, *this, kieli, "EFKT.txt");
        peli->atis.lue_paineet("data/painerajat.txt");
        peli->atis.lahtokiitotie = 0;
        peli->atis.laskukiitotie = 1;
        peli->atis.ok = true;
        peli->atis.siirtokorkeus = 5000;

        PeliView* peliView = new PeliView(asetukset, *peli, *this, kieli);

    	peli->luo_kone();
	    peli->luo_kone();
        PeliController *peliController = new PeliController(*peli, *peliView, *this, asetukset, kieli, dummyPinta);

        peliWidget = new PeliWidget(*peli, *peliView, *peliController, asetukset, *this, kieli, this);

		stack = new QStackedWidget();
		stack->addWidget(levelMenu);
		stack->addWidget(atisView);
		stack->addWidget(peliWidget);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(stack);
		setLayout(layout);

		//stack->setCurrentIndex(2);

		connect(levelMenu, SIGNAL(levelSelected(int)), this, SLOT(OnLevelSelected(int)));
		connect(atisView, SIGNAL(atisDone()), this, SLOT(OnAtisDone()));
	}

	public slots:
	void OnLevelSelected(int level)
	{
		//close();
		stack->setCurrentIndex(1);
	}

	void OnAtisDone()
	{
		stack->setCurrentIndex(2);
		//close();
	}

	double sekunnit(bool nollaa = false)
    {
        return 0;
    }
	
    void odota(unsigned int ms = 1)
    {
    }

	nappi odota_nappi()
    {
        return nappi::NAPPI_MUU;
    }

	bool lue_nappi(nappi n)
    {
        return false;
    }

	nappi lue_nappi()
    {
        return nappi::NAPPI_MUU;
    }

	void tyhjenna_syote()
    {
    }

	apuvalineet::piste anna_hiiri()
    {
        apuvalineet::piste tmp;
        tmp.x = 0;
        tmp.y = 0;
        return tmp;
    }

    bool lue_hiiri()
    {
        return false;
    }

	std::string lue_syote()
    {
        return "";
    }

	void tyhjenna_viesti()
    {
    }

	std::string anna_viesti() 
    {
        return "";
    }

private:
	LevelMenu* levelMenu;
	AtisView* atisView;
	PeliView * peliView;
    PeliWidget* peliWidget;
	QStackedWidget* stack;

    Asetukset asetukset;
    Kieli kieli;
    QPainterPiirtoPinta dummyPinta;
};