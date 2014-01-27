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

#include "levelwidget.hpp"
#include "AtisWidget.h"
#include "PeliView.h"
#include "Metar.hpp"
#include "PeliWidget.h"
#include "peli.hpp"
#include "kieli.hpp"
#include "asetukset.h"
#include "pelicontroller.hpp"
#include "QpiirtoPinta.h"
#include "TiedostoPeliTallenne.hpp"
#include "tilastoView.hpp"
#include "TilastoWidget.hpp"

class MainWindow : public QDialog {
	Q_OBJECT
public:
    MainWindow(Kieli& k) : kieli(k) {
        atis = new Atis;
        metar = new Metar;
        asetukset = new Asetukset;
        tallenne = new TiedostoPeliTallenne;

        tilastoView = new TilastoView(*peli, kieli);
        tilastoWidget = new TilastoWidget(*tilastoView);

        peli = new Peli(*asetukset, kieli, std::string("EFRO.txt"), *atis, *metar);

        peliView = new PeliView(*peli, kieli, *asetukset, *atis);
        dummyPinta = new QPainterPiirtoPinta;
        peliController = new PeliController(*peli, *asetukset, *dummyPinta, *atis);
        peliWidget = new PeliWidget(*peliView, *peliController, *peli, *asetukset);

        levelMenu = new LevelMenu();
        atisWidget = new AtisWidget(*metar, *atis, *peli);

		stack = new QStackedWidget();

        stack->addWidget(levelMenu);
        stack->addWidget(atisWidget);
        stack->addWidget(peliWidget);
        stack->addWidget(tilastoWidget);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(stack);
		setLayout(layout);

		connect(levelMenu, SIGNAL(levelSelected(int)), this, SLOT(OnLevelSelected(int)));
        connect(atisWidget, SIGNAL(atisDone()), this, SLOT(OnAtisDone()));
        connect(peliWidget, SIGNAL(peliDone()), this, SLOT(OnPeliDone()));
	}

    void resizeEvent(QResizeEvent* e) {
        if (this->width() < 700 || this->height() < 600) {
            this->resize(700, 600);
        }

        QWidget::resizeEvent(e);

        asetukset->muuta_asetusta("ruutu_leveys", this->width());
        asetukset->muuta_asetusta("ruutu_korkeus", this->height());
    }

	public slots:
    void OnLevelSelected(int level) {
        stack->setCurrentIndex(1);
        atisWidget->setLevel(level);
        peli->setLevel(level);
	}

    void OnAtisDone() {
        stack->setCurrentIndex(2);

        for (int i = 0; i < (peli->getLevel() * 3); ++i) {
            peli->luo_kone();
        }
    }

    void OnPeliDone() {
        tallenne->tallenna();
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
    TiedostoPeliTallenne* tallenne;

    TilastoWidget* tilastoWidget;
    TilastoView* tilastoView;
};
