#ifndef TILASTOVIEW_HPP
#define TILASTOVIEW_HPP

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include <QTimer>

#include "piirtopinta.hpp"
#include "peli.hpp"
#include "kieli.hpp"

class TilastoView : public QWidget {
    Peli& peli;
    Kieli& kieli;
public:
    TilastoView(Peli& p, Kieli& k) : peli(p), kieli(k) {}

    ~TilastoView() { }

    void piirra(IPiirtoPinta& pinta);
};

#endif // TILASTOVIEW_HPP
