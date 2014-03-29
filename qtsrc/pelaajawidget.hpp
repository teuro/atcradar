#ifndef PELAAJAWIDGET_HPP
#define PELAAJAWIDGET_HPP

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QString>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "apuvalineet.hpp"
#include "peli.hpp"
#include "pelisuorite.hpp"

class Pelaajawidget : public QWidget {
    Q_OBJECT
    Peli& peli;
public:
    Pelaajawidget(Peli& p, QWidget* parent = 0) : peli(p), QWidget(parent) {
        title = new QLabel(tr("Valitse tunnus:"), this);
        title->setGeometry(50, 20, 150, 30);

        QTableWidget* pistetaulu;
        QStringList otsikot;

        tunnus = new QLineEdit("", this);
        tunnus->move(50, 50);

        okButton = new QPushButton(tr("OK"), this);
        okButton->move(50, 80);

        pistetaulu = new QTableWidget(this);
        pistetaulu->move(250, 50);
        pistetaulu->setColumnCount(6);
        pistetaulu->setRowCount(peli.pistevektori.size()+1);
        otsikot << tr("Tunnus") << tr("Pisteet") << tr("Taso") << tr("Porrastusvirheet") << tr("Virheet");
        pistetaulu->setHorizontalHeaderLabels(otsikot);
        pistetaulu->setMinimumWidth(650);
        pistetaulu->setMinimumHeight(500);
        pistetaulu->setEditTriggers(QAbstractItemView::NoEditTriggers);
        pistetaulu->setSelectionBehavior(QAbstractItemView::SelectRows);
        pistetaulu->setSelectionMode(QAbstractItemView::SingleSelection);
        pistetaulu->setShowGrid(true);
        pistetaulu->setStyleSheet("QTableView {selection-background-color: red;}");

        for (unsigned int i = 0; i < peli.pistevektori.size(); ++i) {
            pelisuorite tmp = peli.pistevektori[i];
            pistetaulu->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(peli.pelaajat[tmp.anna_id()].anna_tunnus())));
            pistetaulu->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(tmp.anna_pisteet()))));
            pistetaulu->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(tmp.anna_taso()))));
            pistetaulu->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(tmp.anna_porrastusvirheet()))));
            pistetaulu->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(tmp.anna_virheet()))));
        }

        connect(okButton, SIGNAL(clicked()), this, SLOT(kun_ok_painettu()));
    }

    virtual ~Pelaajawidget() {
        delete title;
        delete okButton;
    }

public slots:
    void kun_ok_painettu() {
        emit pelaaja_valittu(tunnus->text().toStdString());
        //close();
    }

    void paintEvent(QPaintEvent*) {

    }

signals:
    void pelaaja_valittu(std::string);

private:
    QLabel* title;
    QPushButton* okButton;
    QLineEdit* tunnus;
};

#endif // PELAAJAWIDGET_HPP
