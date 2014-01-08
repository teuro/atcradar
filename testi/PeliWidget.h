#ifndef _PELIWIDGET_H_
#define _PELIWIDGET_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

#include "peliview.hpp"
#include "peli.hpp"
#include "pelicontroller.hpp"

class QPainterPiirtoPinta : public IPiirtoPinta {
    QPainter* m_painter;
public:
    void SetPainter(QPainter* painter) {
        m_painter = painter;
    }

    void rectangleColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color) {
        m_painter->drawRect(x1, y1, x2-x1, y2-y1);
    }

    void lineColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color) {
        m_painter->drawLine(x1, y1, x2, y2);
    }

    void circleColor(unsigned short x, unsigned short y, unsigned short rad, unsigned int color) {
        m_painter->drawEllipse(QPointF(x, y), rad, rad);
    }

    void trigonColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, unsigned int color) {
        QPoint points[3] = {QPoint(x1, y1), QPoint(x2, y2), QPoint(x3, y3)};
        m_painter->drawPolygon(points, 3);
    }

    void kirjoita_tekstia(std::string teksti, int x, int y, bool aktiivinen = false) {
        m_painter->drawText(x, y, teksti.c_str());
    }

    int get_fontinkoko() { return 16; }

    void flip() {

    }
};

class PeliWidget : public QWidget {
    Q_OBJECT
    // Frame duration in milliseconds
    static const int frameMs = 1000 / 50;
    QPainterPiirtoPinta piirtopinta;

public:
	// constructor
    PeliWidget(Peli&p, PeliView&v, PeliController& pc, IAsetukset& a, IOhjelma& o, Kieli& kieli, QWidget *aParent = 0) : peli(p), peliView(v), peliController(pc) {
		// Timer to draw the window
		timer = new QTimer;
		connect(timer, SIGNAL(timeout()), SLOT(animate()));
		timer->start(frameMs);

        // To get mouse events continuously even when button is not pressed
        setMouseTracking(true) ;
    }

public slots:
    void animate() {
        peliController.kasittele_aikaa(frameMs / 1000.0);
        update();
    }

    // Redraw the view completely
    void paintEvent(QPaintEvent *) {
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
    PeliView& peliView;
    Peli& peli;
    PeliController& peliController;
};

#endif
