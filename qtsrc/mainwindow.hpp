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
#include "asetukset.hpp"
#include "pelicontroller.hpp"
#include "QTpiirtopinta.hpp"
#include "tilastowidget.hpp"
#include "pelisuorite.hpp"
#include "pelaajawidget.hpp"

class MainWindow : public QDialog {
    Q_OBJECT
public:
    MainWindow() {
        atis = new Atis;
        metar = new Metar;
        asetukset = new Asetukset;

        peli = new Peli(*asetukset, *atis, *metar);

        pelaajaWidget = new Pelaajawidget(*peli);

        peliView = new PeliView(*peli, *asetukset, *atis);
        dummyPinta = new QPainterPiirtoPinta;
        peliController = new PeliController(*peli, *asetukset, *dummyPinta, *atis);
        peliWidget = new PeliWidget(*asetukset, *peliView, *peliController, *peli);

        levelMenu = new LevelMenu();
        atisWidget = new AtisWidget(*metar, *atis, *peli);

        tilastoWidget = new TilastoWidget(*peli);

        stack = new QStackedWidget();

        stack->addWidget(pelaajaWidget);
        stack->addWidget(levelMenu);
        stack->addWidget(atisWidget);
        stack->addWidget(peliWidget);
        stack->addWidget(tilastoWidget);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(stack);
        setLayout(layout);

        connect(pelaajaWidget, SIGNAL(pelaaja_valittu(std::string)), this, SLOT(kun_pelaaja_valittu(std::string)));
        connect(levelMenu, SIGNAL(taso_valittu(int, std::string)), this, SLOT(kun_taso_valittu(int, std::string)));
        connect(atisWidget, SIGNAL(atis_valmis()), this, SLOT(kun_atis_valmis()));
        connect(peliWidget, SIGNAL(peli_valmis()), this, SLOT(kun_peli_valmis()));
        connect(peliWidget, SIGNAL(porrastusvirheet()), this, SLOT(porrastusvirheet_taynna()));
        connect(peliWidget, SIGNAL(tallennettu()), this, SLOT(kun_tallennettu()));
        connect(tilastoWidget, SIGNAL(kunKatsottu()), this, SLOT(palaaPeliin()));
    }

    void resizeEvent(QResizeEvent* e) {
        QWidget::resizeEvent(e);

        asetukset->muuta_asetusta("ruutu_leveys", this->width());
        asetukset->muuta_asetusta("ruutu_korkeus", this->height());

        apuvalineet::ruutu_leveys = asetukset->anna_asetus("ruutu_leveys");
    }

public slots:
    void kun_pelaaja_valittu(std::string tunnus) {
        peli->aseta_pelaaja(tunnus);
        stack->setCurrentIndex(1);
    }

    void kun_taso_valittu(int taso, std::string kentta) {
        peli->lataa_kentta(kentta);
        stack->setCurrentIndex(2);
        atisWidget->aseta_taso(taso);
        peli->aseta_taso(taso);
        asetukset->muuta_asetusta("vaadittavat_kasitellyt", asetukset->anna_asetus("vaadittavat_kasitellyt") * taso);
        asetukset->muuta_asetusta("maks_konemaara", asetukset->anna_asetus("maks_konemaara") * taso);
	}

    void kun_atis_valmis() {
        stack->setCurrentIndex(3);
        peli->aloita();

        for (int i = 0; i < (peli->anna_taso() * 3); ++i) {
            peli->luo_kone();
        }
    }

    void kun_peli_valmis() {
        std::ofstream ulos("pisteet.txt", std::ios::out);

        if (!ulos) {
            throw std::runtime_error(tr("Pisteiden tallennus ei onnistunut").toStdString());
        }

        ulos << peli->anna_taso() << "|" << tilastoWidget->anna_pistesumma() << peli->ajat.size() << "|" << std::endl;
        ulos.close();

        stack->setCurrentIndex(4);
    }

    void porrastusvirheet_taynna() {
        stack->setCurrentIndex(4);
    }

    void kun_tallennettu() {
        stack->setCurrentIndex(4);
    }

    void palaaPeliin() {
        stack->setCurrentIndex(3);
    }

private:
    LevelMenu* levelMenu;
    AtisWidget* atisWidget;
    Peli* peli;
    Atis* atis;

    Metar* metar;
    Asetukset* asetukset;

    QStackedWidget* stack;

    PeliView* peliView;
    QPainterPiirtoPinta* dummyPinta;
    PeliController *peliController;
    PeliWidget* peliWidget;
    Pelaajawidget* pelaajaWidget;

    TilastoWidget* tilastoWidget;
};
