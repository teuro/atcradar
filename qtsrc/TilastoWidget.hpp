#ifndef TILASTOWIDGET_HPP
#define TILASTOWIDGET_HPP

#include <QtWidgets/QWidget>
#include <QTableWidget>
#include <QStringList>
#include <QLabel>
#include <QPushButton>

#include "peli.hpp"

class TilastoWidget : public QWidget {
    Q_OBJECT
    Peli& peli;
    QTableWidget* tilastot;
    QStringList otsikot;
    QPushButton* peliin;

    int pistesumma;
    int selvityssumma;
    double aluesumma;
public:
    // constructor
    TilastoWidget(Peli& p) : peli(p) {
        tilastot = new QTableWidget(this);
        tilastot->move(20, 30);
        tilastot->setColumnCount(6);
        tilastot->setRowCount(1);
        otsikot << tr("Tunnus") << tr("Aluelle") << tr("Alueelta") << tr("Alueella") << tr("Selvitykset") << tr("Pisteet");
        tilastot->setHorizontalHeaderLabels(otsikot);
        tilastot->setMinimumWidth(650);
        tilastot->setMinimumHeight(500);
        tilastot->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tilastot->setSelectionBehavior(QAbstractItemView::SelectRows);
        tilastot->setSelectionMode(QAbstractItemView::SingleSelection);
        tilastot->setShowGrid(true);
        tilastot->setStyleSheet("QTableView {selection-background-color: red;}");

        peliin = new QPushButton(tr("Peliin"), this);
        peliin->move(0, 0);

        connect(peliin, SIGNAL(pressed()), this, SLOT(kun_peliin()));
    }

    int anna_pistesumma() {
       return this->pistesumma;
    }

signals:
    void kunKatsottu();

public slots:
    void kun_peliin() {
        emit kunKatsottu();
    }

    // Redraw the view completely
    void paintEvent(QPaintEvent*) {
        tilastot->setRowCount(peli.ajat.size());
        double alueella;

        for (unsigned int i = 0; i < peli.ajat.size(); ++i) {
            tilastot->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(peli.ajat[i].tunnus)));
            tilastot->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].sisaan))));
            tilastot->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].pois))));
            alueella = peli.ajat[i].pois - peli.ajat[i].sisaan;
            tilastot->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(apuvalineet::muotoile_aika("%m:%s", alueella))));
            tilastot->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].selvitykset))));
            tilastot->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].pisteet))));
        }

        aluesumma       = laske_sarakkeen_summa(*tilastot, 3);
        selvityssumma   = laske_sarakkeen_summa(*tilastot, 4);
        pistesumma      = laske_sarakkeen_summa(*tilastot, 5);

        pistesumma -= (peli.muut_virheet * 10 + peli.porrastusvirheet * 20);

        tilastot->setRowCount(peli.ajat.size()+1);

        tilastot->setItem(peli.ajat.size(), 0, new QTableWidgetItem(tr("Summa")));
        tilastot->setItem(peli.ajat.size(), 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(selvityssumma))));
        tilastot->setItem(peli.ajat.size(), 5, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(pistesumma))));

        tilastot->setRowCount(peli.ajat.size()+2);

        tilastot->setItem(peli.ajat.size()+1, 0, new QTableWidgetItem(tr("Keskiarvo")));
        tilastot->setItem(peli.ajat.size()+1, 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(selvityssumma / peli.ajat.size()))));
        tilastot->setItem(peli.ajat.size()+1, 5, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(pistesumma / peli.ajat.size()))));
        tilastot->setItem(peli.ajat.size()+1, 3, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(aluesumma / peli.ajat.size()))));
    }

    double laske_sarakkeen_summa(QTableWidget& taulu, int sarake) {
        int summa = 0;
        for (int i = 0; i < taulu.rowCount(); ++i) {
            summa += taulu.item(i, sarake)->text().toInt();
        }
        return summa;
    }
};


#endif

