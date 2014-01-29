#ifndef QPIIRTOPINTA_H
#define QPIIRTOPINTA_H

#include "piirtopinta.hpp"

#include <QColor>
#include <map>
#include <QString>

class QPainterPiirtoPinta : public IPiirtoPinta {
    QPainter* m_painter;
    std::map <int, QColor*> colors;
public:
    void SetPainter(QPainter* painter) {
        m_painter = painter;

        colors[IPiirtoPinta::NORMAALI] = new QColor(30, 30, 30);
        colors[IPiirtoPinta::OK] = new QColor(30, 150, 30);
        colors[IPiirtoPinta::VIRHE] = new QColor(150, 30, 30);
        colors[IPiirtoPinta::VALITTU] = new QColor(60, 180, 60);
    }

    ~QPainterPiirtoPinta() {
        //delete m_painter;
    }

    void rectangleColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color) {
        m_painter->setPen(*colors[color]);
        m_painter->drawRect(x1, y1, x2-x1, y2-y1);
    }

    void lineColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned int color) {
        m_painter->setPen(*colors[color]);
        m_painter->drawLine(x1, y1, x2, y2);
    }

    void circleColor(unsigned short x, unsigned short y, unsigned short rad, unsigned int color) {
        m_painter->setPen(*colors[color]);
        m_painter->drawEllipse(QPointF(x, y), rad, rad);
    }

    void trigonColor(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short x3, unsigned short y3, unsigned int color = 0) {
        m_painter->setPen(*colors[color]);
        QPoint points[3] = {QPoint(x1, y1), QPoint(x2, y2), QPoint(x3, y3)};
        m_painter->drawPolygon(points, 3);
    }

    void kirjoita_tekstia(std::string teksti, int x, int y, unsigned int color) {
        m_painter->setPen(*colors[color]);
        m_painter->drawText(x, y, teksti.c_str());
    }

    int get_fontinkoko() { return 16; }

    void flip() { }
};


#endif // QPIIRTOPINTA_H
