#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

#include <fstream>
#include <iostream>

#include "levelmenu.h"
#include "AtisWidget.h"
#include "PeliView.h"
#include "Metar.hpp"
#include "PeliWidget.h"
#include "peli.hpp"
#include "kieli.hpp"
#include "asetukset.h"
#include "pelicontroller.hpp"
#include "QpiirtoPinta.h"

class MainWindow : public QDialog {
	Q_OBJECT
public:
    MainWindow(Kieli& k) : kieli(k) {
        atis = new Atis;
        metar = new Metar;
        asetukset = new Asetukset;

        peli = new Peli(*asetukset, kieli, std::string("EFRO.txt"), *metar, *atis);

        peliView = new PeliView(*peli, kieli, *asetukset, *atis);
        dummyPinta = new QPainterPiirtoPinta;
        peliController = new PeliController(*peli, *asetukset, *dummyPinta, *atis);
        peliWidget = new PeliWidget(*peliView, *peliController, *asetukset, *peli);

        levelMenu = new LevelMenu();
        atisWidget = new AtisWidget(*metar, *atis, *peli);

		stack = new QStackedWidget();

        stack->addWidget(levelMenu);
        stack->addWidget(atisWidget);
        stack->addWidget(peliWidget);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(stack);
		setLayout(layout);

		connect(levelMenu, SIGNAL(levelSelected(int)), this, SLOT(OnLevelSelected(int)));
        connect(atisWidget, SIGNAL(atisDone()), this, SLOT(OnAtisDone()));
	}

    void resizeEvent(QResizeEvent* e) {
        if (this->width() < apuvalineet::ruutu_leveys || this->height() < apuvalineet::ruutu_korkeus) {
            this->resize(apuvalineet::ruutu_leveys, apuvalineet::ruutu_korkeus);
        }

        asetukset->muuta_asetusta("ruutu_leveys", this->width());
        asetukset->muuta_asetusta("ruutu_korkeus", this->height());
    }

	public slots:
    void OnLevelSelected(int level) {
        stack->setCurrentIndex(1);
        atisWidget->setLevel(level);
        peli->setLevel(level);

        asetukset->muuta_asetusta("koska_uusi_ala", 60 / peli->getLevel());
        asetukset->muuta_asetusta("koska_uusi_yla", 120 / peli->getLevel());
    }

    void OnAtisDone() {
        stack->setCurrentIndex(2);
        peli->luo_kone();
        peli->luo_kone();
        peli->luo_kone();
	}

private:
	LevelMenu* levelMenu;
    AtisWidget* atisWidget;
    Peli* peli;
    Atis* atis;

    Metar* metar;
    Asetukset* asetukset;

    Kieli& kieli;

    QStackedWidget* stack;

    PeliView* peliView;
    QPainterPiirtoPinta* dummyPinta;
    PeliController *peliController;
    PeliWidget* peliWidget;
};
