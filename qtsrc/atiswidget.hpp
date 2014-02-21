#ifndef _AtisWidget_H_
#define _AtisWidget_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QValidator>
#include <QToolTip>
#include <QTimer>

#include <iostream>
#include <fstream>
#include <vector>

#include "Metar.hpp"
#include "AtisController.hpp"
#include "peli.hpp"

class AtisWidget : public QWidget {
	Q_OBJECT
public:
    AtisWidget(Metar& m, Atis& at, Peli& p) : metar(m), atis(at), peli(p) {
        otsikko = new QLabel(tr("ATIS Valinta"), this);
        otsikko->setGeometry(0, 20, 150, 30);
        this->taso = 6;
        this->ekaPiirto = true;

        metarkentta = new QLabel(m.getMessage(), this);
        metarkentta->setGeometry(0, 0, 600, 10);

        lisaa_syottokentta(tr("Lähtökiitotie"), 100, 60, 1, 36);
        lisaa_syottokentta(tr("Laskukiitotie"), 100, 80, 1, 36);
        lisaa_syottokentta(tr("Siirtokorkeus"), 100, 100, 3000, 18000);
        lisaa_syottokentta(tr("Siirtopinta"), 100, 120, 30, 220);

        ok_nappi = new QPushButton(tr("OK"), this);
        ok_nappi->move(100, 160);

        connect(ok_nappi, SIGNAL(pressed()), this, SLOT(kun_ok_painettu()));

        ajastin = new QTimer;
        connect(ajastin, SIGNAL(timeout()), SLOT(piirra_avut()));
        ajastin->setInterval(100);
        ajastin->start();
    }

    void lisaa_syottokentta(QString nimi, int x, int y, int alaraja, int ylaraja, QString oletusarvo = "", QString ohjeteksti = "") {
        syottokentat.push_back(new QLineEdit(oletusarvo, this));
        syottokentat.back()->move(x, y);

        syottokentat.back()->setValidator(new QIntValidator(alaraja, ylaraja));

        syottokentta = new QLabel(nimi, this);
        syottokentta->setGeometry(x-100, y-0, 200, 20);

        if (ohjeteksti.length()) {
            ohjekentta = new QLabel(ohjeteksti, this);
            ohjekentta->setGeometry(x+160, y-0, 200, 20);
        }
    }

    void piirra_virheviesti(std::string info, QLineEdit* field) {
        QToolTip::showText(field->mapToGlobal(QPoint()), QString::fromStdString(info));
    }

    void piilota_virheviesti() {
        QToolTip::hideText();
    }

    void aseta_taso(int taso) { this->taso = taso; }

    public slots:

    void piirra_avut() {
        std::string paras_lahto;
        std::string paras_lasku;

        for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
            if (apuvalineet::laske_vastatuuli(peli.kentta.kiitotiet[i].suunta, metar.anna_tuuli()) < 0) {
                paras_lahto = peli.kentta.kiitotiet[i].nimi;
                paras_lasku = peli.kentta.kiitotiet[i].nimi;
                break;
            }
        }

        atis.lataa_painerajat("data/painerajat.txt", syottokentat[2]->text().toInt());
        int siirtopinta = atis.laske_siirtopinta(metar.anna_paine());
        if (this->taso < 3 ) {
            if (this->ekaPiirto) {
                if (this->taso < 2) {
                    syottokentat[0]->setText(QString::fromStdString(paras_lahto));
                    syottokentat[1]->setText(QString::fromStdString(paras_lasku));
                }
                this->ekaPiirto = false;
            }
            syottokentat[3]->setText(QString::fromStdString(apuvalineet::tekstiksi(siirtopinta)));
        }

        atis.tyhjenna();
    }

    void kun_ok_painettu() {
        atis.tyhjenna();
        atis.lataa_painerajat("data/painerajat.txt", syottokentat[2]->text().toInt());

        std::vector <kiitotie> :: iterator haku_lahto = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), syottokentat[0]->text().toStdString());
        std::vector <kiitotie> :: iterator haku_lasku = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), syottokentat[1]->text().toStdString());

        double vasta_lahto = apuvalineet::laske_vastatuuli(syottokentat[0]->text().toInt() * 10, metar.anna_tuuli());
        double vasta_lasku = apuvalineet::laske_vastatuuli(syottokentat[1]->text().toInt() * 10, metar.anna_tuuli());
        int laskettu_siirtopinta = atis.laske_siirtopinta(metar.anna_paine());

        if (haku_lasku == peli.kentta.kiitotiet.end()) {
            piirra_virheviesti("Laskukiitotietä ei ole kentällä", syottokentat[1]);
        } else if (haku_lahto == peli.kentta.kiitotiet.end()) {
            piirra_virheviesti("Lahtokiitotietä ei ole kentällä", syottokentat[0]);
        } else if (laskettu_siirtopinta == 0) {
            piirra_virheviesti("Siirtokorkeus on väärin", syottokentat[2]);
        } else if (vasta_lahto >= 0) {
            piirra_virheviesti("Lähtökiitotie väärin", syottokentat[0]);
        } else if (vasta_lasku >= 0) {
            piirra_virheviesti("Laskukiitotie väärin", syottokentat[1]);
        } else if (laskettu_siirtopinta != syottokentat[3]->text().toInt()) {
            piirra_virheviesti(std::string("Siirtopinta väärin"), syottokentat[3]);
        } else {
            piilota_virheviesti();

            atis.aseta_lahtokiitotie(syottokentat[0]->text().toStdString());
            atis.aseta_laskukiitotie(syottokentat[1]->text().toStdString());
            atis.aseta_siirtokorkeus(syottokentat[2]->text().toInt());
            atis.aseta_siirtopinta(syottokentat[3]->text().toInt());

            emit atis_valmis();
        }
    }

signals:
    void atis_valmis();

private:
    Metar& metar;
    Atis& atis;
    Peli& peli;

    QLabel* otsikko;
    QLabel* syottokentta;
    QLabel* ohjekentta;
    QLabel* metarkentta;
    QLabel* virhe;

    QPushButton* ok_nappi;

    std::vector <QLineEdit*> syottokentat;
    int taso;
    bool ekaPiirto;

    QTimer* ajastin;
};

#endif
