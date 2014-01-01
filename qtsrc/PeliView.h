#ifndef _PELIVIEW_H_
#define _PELIVIEW_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

#include <list>

class lentokone
{
public:
	// In world coordinates
	double x, y;
	double vx, vy;
	double ax, ay;

	void tickTime(double seconds)
	{
		vx += ax * seconds;
		vy += ay * seconds;
		x += vx * seconds;
		y += vy * seconds;
	}
};

class PeliView : public QWidget
{
	Q_OBJECT
	//signals :
	// define mouseClick signal
	//	void mouseClickEvent();

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
	PeliView(QWidget *aParent = 0)
	{
		// Generate a number of objects in random locations with random velocities
		for (int i = 0; i < 15; i++)
		{
			lentokone* lk = new lentokone();
			lk->x = rand() % world_width;
			lk->y = rand() % world_height;

			// Max 300 m/s
			const int maxV = 200;
			lk->vx = rand() % maxV*2 - maxV;
			lk->vy = rand() % maxV*2 - maxV;

			m_lentokoneet.push_back(lk);
		}

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
	void paintEvent(QPaintEvent *)
	{
		QPainter p(this);

		p.setPen(Qt::blue);
		auto fillBrush = QBrush(Qt::black);
		for (std::list<lentokone*>::iterator it = m_lentokoneet.begin(); it != m_lentokoneet.end(); it++) {
			auto lk = *it;
			if (lk == selected_lentokone)
			{
				p.setBrush(fillBrush);
			}
			else {
				p.setBrush(Qt::NoBrush);
			}
			p.drawEllipse(QPointF(lk->x / (double)world_width * (double)viewport_width, lk->y / (double)world_height * (double)viewport_height), 10, 10);
		}

		// Draw line between selected lentokone and current mouse position
		if (selected_lentokone)
		{
			auto lk = selected_lentokone;
			p.setPen(Qt::green);
			p.drawLine(lk->x / (double)world_width * (double)viewport_width, lk->y / (double)world_height * (double)viewport_height, m_endPoint.x(), m_endPoint.y());
		}
	}

	// Mouse button up
	void mouseReleaseEvent(QMouseEvent * e)
	{
		if (selected_lentokone)
		{
			// Unselect any selected lentokone and reset the acceleration
			selected_lentokone->ax = 0;
			selected_lentokone->ay = 0;
			selected_lentokone = NULL;
		}
		update();
	}

	double distance(double x1, double y1, double x2, double y2)
	{
		return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	}

	// Mouse button down
	void mousePressEvent(QMouseEvent * e)
	{
		auto point = e->pos();
		m_endPoint = point;

		selected_lentokone = NULL;

		// Select lentokone under mouse
		for (std::list<lentokone*>::iterator it = m_lentokoneet.begin(); it != m_lentokoneet.end(); it++) 
		{
			if (distance((*it)->x / (double)world_width * (double)viewport_width, (*it)->y / (double)world_height * (double)viewport_height, 
				point.x(), point.y()) <= 10) 
			{
				selected_lentokone = *it;
				break;
			}
		}

		update();
	}

public slots:
	void animate()
	{
		if (selected_lentokone)
		{
			// accelerate lentokone to the direction of mouse location
			selected_lentokone->ax = (double)m_endPoint.x() * (double)world_width / (double)viewport_width - selected_lentokone->x;
			selected_lentokone->ay = (double)m_endPoint.y() * (double)world_height / (double)viewport_height - selected_lentokone->y;
		}

		for (std::list<lentokone*>::iterator it = m_lentokoneet.begin(); it != m_lentokoneet.end(); it++) {
			lentokone*l = *it;

			l->tickTime(frameMs / 1000.0);

			// Bounce off the walls :)
			if (l->x < 0 || l->x > world_width) {
				l->vx = -l->vx;
			}
			if (l->y < 0 || l->y > world_height) {
				l->vy = -l->vy;
			}
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
