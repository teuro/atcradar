#ifndef TILASTOWIDGET_HPP
#define TILASTOWIDGET_HPP

#include <QtWidgets/QWidget>

#include "tilastoView.hpp"
#include "QpiirtoPinta.h"

class TilastoWidget : public QWidget {
    Q_OBJECT

public:
    // constructor
    TilastoWidget(TilastoView& v) : tilastoView(v) {

    }

public slots:
    // Redraw the view completely
    void paintEvent(QPaintEvent*) {
        QPainter p(this);
        piirtopinta.SetPainter(&p);
        tilastoView.piirra(piirtopinta);
    }
private:
    QPainterPiirtoPinta piirtopinta;
    TilastoView& tilastoView;
};


#endif

