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
    PeliWidget(PeliView& v, PeliController& pc, IAsetukset& a, QWidget *aParent = 0) : peliView(v), peliController(pc) {
		// Timer to draw the window
		timer = new QTimer;
		connect(timer, SIGNAL(timeout()), SLOT(animate()));
		timer->start(frameMs);
        aika = new QTime();

        // To get mouse events continuously even when button is not pressed
        setMouseTracking(true);

        addInputField("Suunta", 100, 20, 0, 360);
        addInputField("Nopeus", 100, 40, 160, 440);
        addInputField("Korkeus", 100, 60, 2000, 24000);

        okButton = new QPushButton("OK", this);
        okButton->move(100, 80);

        connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
    }

    void addInputField(QString name, int x, int y, int minimum, int maximum) {
        inputFields.push_back(new QLineEdit("", this));
        inputFields.back()->move(x, y);

        QIntValidator* tmp = new QIntValidator(minimum, maximum, inputFields.back());
        inputFields.back()->setValidator(tmp);

        inputLabel = new QLabel(name, this);
        inputLabel->setGeometry(x-80, y-0, 200, 20);
    }

public slots:
    void animate() {
        peliController.kasittele_aikaa(frameMs / 1000.0);
        peliController.ota_aika(aika->elapsed());
        update();
    }

    void OnOkPressed() {

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

    QLabel* inputLabel;
    QLabel* metarLabel;
    QLabel* error;

    QPushButton* okButton;

    std::vector <QLineEdit*> inputFields;
};


#endif
