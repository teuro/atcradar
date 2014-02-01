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
#include "AtisWidget.hpp"
#include "PeliView.hpp"
#include "Metar.hpp"
#include "PeliWidget.hpp"
#include "peli.hpp"
#include "kieli.hpp"
#include "asetukset.hpp"
#include "pelicontroller.hpp"
#include "QTpiirtopinta.hpp"
#include "tilastoview.hpp"
#include "tilastowidget.hpp"

class MainWindow : public QDialog {
    Q_OBJECT
public:
    MainWindow(Kieli& k) : kieli(k) {
        atis = new Atis;
        metar = new Metar;
        asetukset = new Asetukset;

        peli = new Peli(*asetukset, kieli, std::string("EFRO.txt"), *atis, *metar);

        peliView = new PeliView(*peli, kieli, *asetukset, *atis);
        dummyPinta = new QPainterPiirtoPinta;
        peliController = new PeliController(*peli, *asetukset, *dummyPinta, *atis);
        peliWidget = new PeliWidget(*asetukset, *peliView, *peliController, *peli);

        levelMenu = new LevelMenu();
        atisWidget = new AtisWidget(*metar, *atis, *peli);

        tilastoView = new TilastoView(*peli, kieli);
        tilastoWidget = new TilastoWidget(*tilastoView);

        stack = new QStackedWidget();

        stack->addWidget(levelMenu);
        stack->addWidget(atisWidget);
        stack->addWidget(peliWidget);
        stack->addWidget(tilastoWidget);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(stack);
        setLayout(layout);

        connect(levelMenu, SIGNAL(taso_valittu(int)), this, SLOT(kun_taso_valittu(int)));
        connect(atisWidget, SIGNAL(atis_valmis()), this, SLOT(kun_atis_valmis()));
        connect(peliWidget, SIGNAL(peli_valmis()), this, SLOT(kun_peli_valmis()));
        connect(peliWidget, SIGNAL(porrastusvirheet()), this, SLOT(porrastusvirheet_taynna()));
        connect(peliWidget, SIGNAL(tallennettu()), this, SLOT(kun_tallennettu()));
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
    void kun_taso_valittu(int taso) {
        stack->setCurrentIndex(1);
        atisWidget->aseta_taso(taso);
        peli->aseta_taso(taso);
        asetukset->muuta_asetusta("vaadittavat_kasitellyt", asetukset->anna_asetus("vaadittavat_kasitellyt") * taso);
        asetukset->muuta_asetusta("maks_konemaara", asetukset->anna_asetus("maks_konemaara") * taso);
	}

    void kun_atis_valmis() {
        stack->setCurrentIndex(2);
        peli->aloita();

        for (int i = 0; i < (peli->anna_taso() * 3); ++i) {
            peli->luo_kone();
        }
    }

    void kun_peli_valmis() {
        stack->setCurrentIndex(3);
    }

    void porrastusvirheet_taynna() {
        stack->setCurrentIndex(3);
    }

    void kun_tallennettu() {
        stack->setCurrentIndex(3);
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

    TilastoWidget* tilastoWidget;
    TilastoView* tilastoView;
};
