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
public:
    // constructor
    TilastoWidget(Peli& p) : peli(p) {
        tilastot = new QTableWidget(this);
        tilastot->setColumnCount(6);
        tilastot->setRowCount(1);
        otsikot << "Tunnus" << "Aluelle" << "Alueelta" << "Alueella" << "Selvitykset" << "Pisteet";
        tilastot->setHorizontalHeaderLabels(otsikot);
        tilastot->setMinimumWidth(650);
        tilastot->setMinimumHeight(700);
        tilastot->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tilastot->setSelectionBehavior(QAbstractItemView::SelectRows);
        tilastot->setSelectionMode(QAbstractItemView::SingleSelection);
        tilastot->setShowGrid(false);
        tilastot->setStyleSheet("QTableView {selection-background-color: red;}");

        peliin = new QPushButton("Peliin", this);
        peliin->move(0, 0);

        connect(peliin, SIGNAL(pressed()), this, SLOT(kun_peliin()));
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

        for (unsigned int i = 0; i < peli.ajat.size(); ++i) {
            tilastot->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(peli.ajat[i].tunnus)));
            tilastot->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].sisaan))));
            tilastot->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].pois))));
            tilastot->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].pois - peli.ajat[i].sisaan))));
            tilastot->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].selvitykset))));
            tilastot->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(peli.ajat[i].pisteet))));
        }

        double aluesumma        = laske_sarakkeen_summa(*tilastot, 3);
        double selvityssumma    = laske_sarakkeen_summa(*tilastot, 4);
        double pistesumma       = laske_sarakkeen_summa(*tilastot, 5);

        pistesumma -= (peli.muut_virheet * 10 + peli.porrastusvirheet * 20);

        tilastot->setRowCount(peli.ajat.size()+1);

        tilastot->setItem(peli.ajat.size(), 0, new QTableWidgetItem("Summa"));
        tilastot->setItem(peli.ajat.size(), 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(selvityssumma))));
        tilastot->setItem(peli.ajat.size(), 5, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(pistesumma))));

        tilastot->setRowCount(peli.ajat.size()+2);

        tilastot->setItem(peli.ajat.size()+1, 0, new QTableWidgetItem("Keskiarvo"));
        tilastot->setItem(peli.ajat.size()+1, 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(selvityssumma / peli.ajat.size()))));
        tilastot->setItem(peli.ajat.size()+1, 5, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(pistesumma / peli.ajat.size()))));
        tilastot->setItem(peli.ajat.size()+1, 3, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(aluesumma / peli.ajat.size()))));
    }

    double laske_sarakkeen_summa(QTableWidget& taulu, int sarake) {
        int summa = 0;
        for (int i = 0; i < taulu.rowCount(); ++i) {
            summa += taulu.item(i, sarake)->text().toDouble();
        }
        return summa;
    }
};


#endif

