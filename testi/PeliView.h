#ifndef _PELIVIEW_H_
#define _PELIVIEW_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include <QTimer>

#include "AtisController.hpp"
#include "Metar.hpp"
#include "lentokone.hpp"

#include <list>
#include <cmath>

class PeliView : public QWidget {
	Q_OBJECT
	// World area dimensions in meters
	static const int world_width = 10000;
	static const int world_height = 10000;

	// Display are dimensions in pixels
	static const int viewport_width = 700;
	static const int viewport_height = 700;

	// Frame duration in milliseconds
	static const int frameMs = 1000 / 50;

public:
	// constructor
    PeliView(QWidget* aParent = 0) {
		// Timer to draw the window
		timer = new QTimer;
		connect(timer, SIGNAL(timeout()), SLOT(animate()));
		timer->start(frameMs);
        selected_lentokone = NULL;
	}

protected:
	void mouseMoveEvent(QMouseEvent* e) {
		// Mouse has moved while the user presses the mouse button
		// We store mouse position and redraw the screen
		m_endPoint = e->pos();
		update();
	}

	// Redraw the view completely
    void paintEvent(QPaintEvent *) {
		QPainter p(this);

		p.setPen(Qt::blue);
        QBrush fillBrush = QBrush(Qt::black);
		for (std::list<lentokone*>::iterator it = m_lentokoneet.begin(); it != m_lentokoneet.end(); it++) {
            lentokone* lk = *it;
			if (lk == selected_lentokone)
			{
				p.setBrush(fillBrush);
			}
			else {
				p.setBrush(Qt::NoBrush);
			}
            p.drawEllipse(QPointF(lk->anna_paikka().x / (double)world_width * (double)viewport_width, lk->anna_paikka().y / (double)world_height * (double)viewport_height), 10, 10);
		}

		// Draw line between selected lentokone and current mouse position
		if (selected_lentokone)
		{
            lentokone* lk = selected_lentokone;
			p.setPen(Qt::green);
            p.drawLine(lk->anna_paikka().x / (double)world_width * (double)viewport_width, lk->anna_paikka().y / (double)world_height * (double)viewport_height, m_endPoint.x(), m_endPoint.y());
		}
	}

	// Mouse button up
    void mouseReleaseEvent(QMouseEvent * e) {

	}

    double distance(double x1, double y1, double x2, double y2) {
        return std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    }

	// Mouse button down
    void mousePressEvent(QMouseEvent * e) {
        QPoint point = e->pos();
		m_endPoint = point;

        selected_lentokone = NULL;

		// Select lentokone under mouse
        for (std::list<lentokone*>::iterator it = m_lentokoneet.begin(); it != m_lentokoneet.end(); it++) {
            if (distance((*it)->anna_paikka().x / (double)world_width * (double)viewport_width, (*it)->anna_paikka().y / (double)world_height * (double)viewport_height, point.x(), point.y()) <= 10) {
				selected_lentokone = *it;
				break;
            } else {
                selected_lentokone = NULL;
            }
		}

		update();
	}

public slots:
    void animate() {
		for (std::list<lentokone*>::iterator it = m_lentokoneet.begin(); it != m_lentokoneet.end(); it++) {
            lentokone* l = *it;

            l->liiku(frameMs);
		}

		// simply repaint
		update();
	}

private:
	// animation timer
	QTimer *timer;

	// member variable to store click position
	QPoint m_endPoint;
	// member variable - flag of click beginning
	bool m_mouseClick;

	std::list<lentokone*> m_lentokoneet;
	lentokone* selected_lentokone;
};

#endif
