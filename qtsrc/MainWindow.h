#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

#include <fstream>

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
        peliController = new PeliController(*peli, *asetukset, *dummyPinta);
        peliWidget = new PeliWidget(*peliView, *peliController, *asetukset);

		levelMenu = new LevelMenu();
        atisView = new AtisView(*metar, *atis);

		stack = new QStackedWidget();

		stack->addWidget(levelMenu);
		stack->addWidget(atisView);
        stack->addWidget(peliWidget);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(stack);
		setLayout(layout);

		connect(levelMenu, SIGNAL(levelSelected(int)), this, SLOT(OnLevelSelected(int)));
		connect(atisView, SIGNAL(atisDone()), this, SLOT(OnAtisDone()));
	}

	public slots:
    void OnLevelSelected(int level) {
        std::clog << "Tasoksi valittu " << level << std::endl;
		stack->setCurrentIndex(1);
	}

    void OnAtisDone() {
        stack->setCurrentIndex(2);
        std::clog << "Atis täytetty oikein" << std::endl;
        peli->luo_kone(0);
        peli->luo_kone(0);
	}

private:
	LevelMenu* levelMenu;
	AtisView* atisView;
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
