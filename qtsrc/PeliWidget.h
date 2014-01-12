#ifndef _PELIWIDGET_H_
#define _PELIWIDGET_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QLineEdit>
#include <QValidator>
#include <QPainter>
#include <QTimer>
#include <QTime>

#include "peli.hpp"
#include "PeliView.h"
#include "pelicontroller.hpp"
#include "QpiirtoPinta.h"

class PeliWidget : public QWidget {
    Q_OBJECT
    // Frame duration in milliseconds
    static const int frameMs = 1000 / 50;
    QPainterPiirtoPinta piirtopinta;

public:
	// constructor
    PeliWidget(PeliView& v, PeliController& pc, IAsetukset& a, Peli& p) : peliView(v), peliController(pc), peli(p) {
		// Timer to draw the window
		timer = new QTimer;
		connect(timer, SIGNAL(timeout()), SLOT(animate()));
		timer->start(frameMs);
        aika = new QTime();

        addInputField("Suunta", 100, 20, 0, 360);
        addInputField("Nopeus", 100, 40, 160, 440);
        addInputField("Korkeus", 100, 60, 2000, 24000);

        okButton = new QPushButton("OK", this);
        okButton->move(100, 80);

        lahesty = new QPushButton("Lähesty", this);
        lahesty->move(200, 80);

        keskeyta = new QPushButton("Keskeyta", this);
        keskeyta->move(200, 80);

        oikotie = new QPushButton("Oikotie", this);
        oikotie->move(200, 80);

        std::clog << connect(okButton, SIGNAL(pressed()), this, SLOT(OnOkPressed())) << std::endl;
        std::clog << connect(lahesty, SIGNAL(pressed()), this, SLOT(OnApproach())) << std::endl;
        std::clog << connect(keskeyta, SIGNAL(pressed()), this, SLOT(OnCancel())) << std::endl;
        std::clog << connect(oikotie, SIGNAL(pressed()), this, SLOT(OnShortCut())) << std::endl;

        // To get mouse events continuously even when button is not pressed
        setMouseTracking(true);
    }

    void addInputField(QString name, int x, int y, int minimum, int maximum) {
        inputField tmp;

        tmp.field = new QLineEdit("", this);
        tmp.label = new QLabel(name, this);

        tmp.field->move(x, y);
        tmp.label->setGeometry(x-80, y-0, 200, 20);

        inputFields.push_back(tmp);
    }

public slots:
    void animate() {
        peliController.kasittele_aikaa(frameMs / 1000.0);
        peliController.ota_aika(aika->elapsed());

        lahesty->hide();
        keskeyta->hide();
        oikotie->hide();

        if (peli.valittuKone) {
            if (peli.valittuKone->tyyppi == Peli::LAHTEVA) {
                oikotie->show();
            } else if (peli.valittuKone->tyyppi == Peli::SAAPUVA) {
                if (peli.valittuKone->lahestymisselvitys) {
                    keskeyta->show();
                } else {
                    lahesty->show();
                }
            }
        }

        for (std::vector <inputField> :: iterator it = inputFields.begin(); it != inputFields.end(); ++it) {
            if (peli.valittuKone && !it->field->isVisible()) {
                it->label->show();
                it->field->show();
                okButton->show();
                it->field->setFocus();
            }
        }

        update();
    }

    void OnApproach() {
        std::clog << "Lähesty" << std::endl;
        peliController.kasittele_komento("ILS|" + apuvalineet::tekstiksi(apuvalineet::LAHESTYMIS) + "|");
        peli.valittuKone = NULL;
    }

    void OnCancel() {
        std::clog << "Keskeytä" << std::endl;
        peliController.kasittele_komento("CNL|" + apuvalineet::tekstiksi(apuvalineet::LAHESTYMIS) + "|");
        peli.valittuKone = NULL;
    }

    void OnShortCut() {
        std::clog << "Oikotie" << std::endl;
        peliController.kasittele_komento("DCT|" + apuvalineet::tekstiksi(apuvalineet::OIKOTIE) + "|");
        peli.valittuKone = NULL;
    }

    void OnOkPressed() {
        std::clog << "OK" << std::endl;
        std::string komento;
        int i = 1;

        for (std::vector <inputField> :: iterator it = inputFields.begin(); it != inputFields.end(); ++it) {
            it->label->hide();
            it->field->hide();
            komento += it->field->text().toStdString() + "|" + apuvalineet::tekstiksi(i) + "|";
            ++i;
        }

        okButton->hide();

        peliController.kasittele_komento(komento);
        peli.valittuKone = NULL;
    }

    // Redraw the view completely
    void paintEvent(QPaintEvent*) {
        QPainter p(this);
        piirtopinta.SetPainter(&p);
        peliView.piirra(piirtopinta);
    }

    // Mouse button up
    void mouseReleaseEvent(QMouseEvent* e) {
        QPoint pos = e->pos();
        peliController.kasittele_hiiren_nappi(apuvalineet::piste(pos.x(), pos.y()));

        update();
    }

    void mouseMoveEvent(QMouseEvent* e) {
        // Mouse has moved while the user presses the mouse button
        QPoint pos = e->pos();
        peliController.kasittele_hiiren_paikka(apuvalineet::piste(pos.x(), pos.y()));
    }

private:
    // animation timer
    QTimer *timer;
    QTime* aika;

    PeliView& peliView;
    PeliController& peliController;
    Peli& peli;

    QLabel* inputLabel;
    QLabel* metarLabel;
    QLabel* error;

    QPushButton* okButton;
    QPushButton* lahesty;
    QPushButton* keskeyta;
    QPushButton* oikotie;

    struct inputField {
        QLineEdit* field;
        QLabel* label;
    };

    std::vector <inputField> inputFields;

};


#endif
