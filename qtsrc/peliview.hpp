#ifndef _PELIVIEW_HPP
#define _PELIVIEW_HPP

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QObject>
#include <QTableWidget>
#include <QStringList>

#include "AtisController.hpp"
#include "Metar.hpp"
#include "piirtopinta.hpp"
#include "peli.hpp"
#include "asetukset.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <list>
#include <cmath>

/*
MVC View
PeliView is responsible of DISPLAYING the game state. Nothing else.
*/

class PeliView : public QWidget {
    Peli& peli;
    Asetukset& asetukset;
    Atis& atis;
    QTableWidget* muuttujat;
    QStringList otsikot;
public:
    PeliView(Peli& p, Asetukset& a, Atis& at) : peli(p), asetukset(a), atis(at) {

    }

    ~PeliView() { }

    void piirra(IPiirtoPinta& pinta);
private:
    void piirra_koneet(IPiirtoPinta& piirtopinta);
    void piirra_navipisteet(IPiirtoPinta& piirtopinta);
    void piirra_lentokentta(IPiirtoPinta& piirtopinta);
    void piirra_tilanne(IPiirtoPinta& piirtopinta);
    void piirra_metar(IPiirtoPinta& piirtopinta);
    void piirra_odottavat(IPiirtoPinta& piirtopinta);
    void piirra_ohje(IPiirtoPinta& piirtopinta, std::string ohje);
    void piirra_jaljet(IPiirtoPinta& piirtopinta);
};

#endif
