#ifndef _PELIWIDGET_H_
#define _PELIWIDGET_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>

#include "PeliView.h"
#include "peli.hpp"
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
        setMouseTracking(true) ;
    }

public slots:
    void animate() {
        peliController.kasittele_aikaa(frameMs / 1000.0);
        peliController.ota_aika(aika->elapsed());
        update();
    }

    // Redraw the view completely
    void paintEvent(QPaintEvent*) {
        QPainter p(this);
        piirtopinta.SetPainter(&p);
        peliView.piirra(piirtopinta);
    }

    // Mouse button up
    void mouseReleaseEvent(QMouseEvent * e) {
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
};


#endif
