#ifndef PELAAJAWIDGET_HPP
#define PELAAJAWIDGET_HPP

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
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

        tunnus = new QLineEdit("", this);
        tunnus->move(50, 50);

        okButton = new QPushButton(tr("OK"), this);
        okButton->move(50, 80);

        int y = 50;

        for (unsigned int i = 0; i < peli.pistevektori.size(); ++i) {

            QLabel* pistetunnus = new QLabel(QString::fromStdString(peli.pelaajat[peli.pistevektori[i].anna_id()].anna_tunnus()), this);
            pistetunnus->move(220, y);

            QLabel* pistearvo = new QLabel(QString::fromStdString(apuvalineet::tekstiksi(peli.pistevektori[i].anna_pisteet())), this);
            pistearvo->move(270, y);

            y += 20;
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

signals:
    void pelaaja_valittu(std::string);

private:
    QLabel* title;
    QPushButton* okButton;
    QLineEdit* tunnus;
};

#endif // PELAAJAWIDGET_HPP
