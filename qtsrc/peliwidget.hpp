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
#include <QResizeEvent>

#include "peli.hpp"
#include "PeliView.hpp"
#include "pelicontroller.hpp"
#include "QTpiirtopinta.hpp"
#include "asetukset.hpp"

class PeliWidget : public QWidget {
    Q_OBJECT
    // Frame duration in milliseconds
    static const int frameMs = 1000 / 50;
    QPainterPiirtoPinta piirtopinta;
    IAsetukset& asetukset;

public:
	// constructor
    PeliWidget(IAsetukset& a, PeliView& v, PeliController& pc, Peli& p) :asetukset(a), peliView(v), peliController(pc), peli(p) {
		// Timer to draw the window
		timer = new QTimer;
        connect(timer, SIGNAL(timeout()), SLOT(animate()));
        timer->start(frameMs);

        addInputField(tr("Suunta"), 100, 20, apuvalineet::SUUNTA);
        addInputField(tr("Nopeus"), 100, 40, apuvalineet::NOPEUS);
        addInputField(tr("Korkeus"), 100, 60, apuvalineet::KORKEUS);

        okButton = new QPushButton(tr("OK"), this);
        okButton->move(100, 80);

        lahesty = new QPushButton(tr("Lähesty"), this);
        lahesty->move(200, 80);

        keskeyta = new QPushButton(tr("Keskeyta"), this);
        keskeyta->move(200, 80);

        oikotie = new QPushButton(tr("Oikotie"), this);
        oikotie->move(200, 80);

        tilastot = new QPushButton(tr("Tilastot"), this);
        tilastot->move(0, 0);

        connect(okButton, SIGNAL(pressed()), this, SLOT(OnOkPressed()));
        connect(lahesty, SIGNAL(pressed()), this, SLOT(OnApproach()));
        connect(keskeyta, SIGNAL(pressed()), this, SLOT(OnCancel()));
        connect(oikotie, SIGNAL(pressed()), this, SLOT(OnShortCut()));
        connect(tilastot, SIGNAL(pressed()), this, SLOT(kun_tilastot()));

        // To get mouse events continuously even when button is not pressed
        setMouseTracking(true);
    }

    void addInputField(QString name, int x, int y, int type) {
        inputField tmp;

        tmp.field = new QLineEdit("", this);
        tmp.label = new QLabel(name, this);

        tmp.field->move(x, y);
        tmp.label->setGeometry(x-80, y-0, 200, 20);

        tmp.type = type;

        inputFields.push_back(tmp);
    }

signals:
    void peli_valmis();
    void porrastusvirheet();
    void tallennettu();

public slots:
    void animate() {
        if (peli.kasitellyt >= asetukset.anna_asetus("vaadittavat_kasitellyt")) {
            if (peli.koneet.size() == 0) {
                peli.muodosta_suorite();
                emit peli_valmis();
            }
        } else if (peli.porrastusvirheet >= asetukset.anna_asetus("maks_porrastusvirhe")) {
            emit porrastusvirheet();
        }

        peliController.kasittele_aikaa(frameMs / 1000.0);

        lahesty->hide();
        keskeyta->hide();
        oikotie->hide();
        okButton->hide();

        if (peli.valittuKone) {
            okButton->show();
            if (peli.valittuKone->tyyppi == Peli::LAHTEVA && !peli.valittuKone->anna_oikotie()) {
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
            }
        }

        update();
    }

    void OnApproach() {
        peliController.kasittele_komento("ILS|" + apuvalineet::tekstiksi(apuvalineet::LAHESTYMIS) + "|");
        peli.valittuKone = NULL;
    }

    void OnCancel() {
        peliController.kasittele_komento("CNL|" + apuvalineet::tekstiksi(apuvalineet::KESKEYTA) + "|");
        peli.valittuKone = NULL;
    }

    void OnShortCut() {
        peliController.kasittele_komento("DCT|" + apuvalineet::tekstiksi(apuvalineet::OIKOTIE) + "|");
        peli.valittuKone = NULL;
    }

    void OnOkPressed() {
        std::string komento;
        int i = 1;

        for (std::vector <inputField> :: iterator it = inputFields.begin(); it != inputFields.end(); ++it) {
            it->label->hide();
            it->field->hide();
            komento += it->field->text().toStdString() + "|" + apuvalineet::tekstiksi(it->type) + "|";
            ++i;
        }

        okButton->hide();

        peliController.kasittele_komento(komento);
        peli.valittuKone = NULL;
    }

    void kun_tilastot() {
        emit tallennettu();
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

        if (peli.valittuKone) {
            inputFields[0].field->setText(QString::fromStdString(apuvalineet::tekstiksi(peli.valittuKone->anna_selvityssuunta())));
            inputFields[1].field->setText(QString::fromStdString(apuvalineet::tekstiksi(peli.valittuKone->anna_selvitysnopeus())));
            inputFields[2].field->setText(QString::fromStdString(apuvalineet::tekstiksi(peli.valittuKone->anna_selvityskorkeus())));
        } else {
            for (unsigned int i = 0; i < inputFields.size(); ++i) {
                inputFields[i].field->hide();
                inputFields[i].label->hide();
            }
        }

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
    QPushButton* tilastot;

    struct inputField {
        QLineEdit* field;
        QLabel* label;
        int type;
    };

    std::vector <inputField> inputFields;
};


#endif
