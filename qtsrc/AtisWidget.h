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
		title = new QLabel("ATIS Valinta", this);
        title->setGeometry(0, 20, 150, 30);
        this->level = 6;
        this->ekaPiirto = true;

        metarLabel = new QLabel(m.getMessage(), this);
        metarLabel->setGeometry(0, 0, 600, 10);

        addInputField("Lähtökiitotie", 100, 60, 1, 36);
        addInputField("Laskukiitotie", 100, 80, 1, 36);
        addInputField("Siirtokorkeus", 100, 100, 3000, 18000);
        addInputField("Siirtopinta", 100, 120, 30, 220);

        okButton = new QPushButton("OK", this);
        okButton->move(100, 160);

        connect(okButton, SIGNAL(pressed()), this, SLOT(OnOkPressed()));

        timer = new QTimer;
        connect(timer, SIGNAL(timeout()), SLOT(displayHelpers()));
        timer->setInterval(100);
        timer->start();
    }

    void addInputField(QString name, int x, int y, int minimum, int maximum, QString defaultValue = "", QString infoText = "") {        
        inputFields.push_back(new QLineEdit(defaultValue, this));
        inputFields.back()->move(x, y);

        inputLabel = new QLabel(name, this);
        inputLabel->setGeometry(x-80, y-0, 200, 20);

        inputFields.back()->setValidator(new QIntValidator(minimum, maximum, inputFields.back()));

        if (infoText.length()) {
            infoLabel = new QLabel(infoText, this);
            infoLabel->setGeometry(x+160, y-0, 200, 20);
        }
    }

    void drawErrorMessage(std::string info, QLineEdit* field) {
        QToolTip::showText(field->mapToGlobal(QPoint()), QString::fromStdString(info));
    }

    void hideErrorMessage() {
        QToolTip::hideText();
    }

    void setLevel(int level) { this->level = level; }

    public slots:

    void displayHelpers() {
        std::string paras_lahto;
        std::string paras_lasku;

        for (unsigned int i = 0; i < peli.kentta.kiitotiet.size(); ++i) {
            if (apuvalineet::laske_vastatuuli(peli.kentta.kiitotiet[i].suunta, metar.anna_tuuli()) < 0) {
                paras_lahto = peli.kentta.kiitotiet[i].nimi;
                paras_lasku = peli.kentta.kiitotiet[i].nimi;
                break;
            }
        }

        atis.downloadPrressureLimit("data/painerajat.txt", inputFields[2]->text().toInt());
        int siirtopinta = atis.calculateTL(metar.anna_paine());
        if (this->level < 2 ) {
            if (this->ekaPiirto) {
                inputFields[0]->setText(QString::fromStdString(paras_lahto));
                inputFields[1]->setText(QString::fromStdString(paras_lasku));
                this->ekaPiirto = false;
            }

            inputFields[3]->setText(QString::fromStdString(apuvalineet::tekstiksi(siirtopinta)));
        }
        atis.tyhjenna();
    }

    void OnOkPressed() {
        atis.tyhjenna();
        atis.downloadPrressureLimit("data/painerajat.txt", inputFields[2]->text().toInt());

        std::vector <kiitotie> :: iterator haku_lahto = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), inputFields[0]->text().toStdString());
        std::vector <kiitotie> :: iterator haku_lasku = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), inputFields[1]->text().toStdString());

        double vasta_lahto = apuvalineet::laske_vastatuuli(inputFields[0]->text().toInt() * 10, metar.anna_tuuli());
        double vasta_lasku = apuvalineet::laske_vastatuuli(inputFields[1]->text().toInt() * 10, metar.anna_tuuli());
        int laskettu_siirtopinta = atis.calculateTL(metar.anna_paine());

        if (haku_lasku == peli.kentta.kiitotiet.end()) {
            drawErrorMessage("Laskukiitotietä ei ole kentällä", inputFields[1]);
        } else if (haku_lahto == peli.kentta.kiitotiet.end()) {
            drawErrorMessage("Lahtokiitotietä ei ole kentällä", inputFields[0]);
        } else if (laskettu_siirtopinta == 0) {
            drawErrorMessage("Siirtokorkeus on väärin", inputFields[2]);
        } else if (vasta_lahto >= 0) {
            drawErrorMessage("Lähtökiitotie väärin", inputFields[0]);
        } else if (vasta_lasku >= 0) {
            drawErrorMessage("Laskukiitotie väärin", inputFields[1]);
        } else if (laskettu_siirtopinta != inputFields[3]->text().toInt()) {
            drawErrorMessage(std::string("Siirtopinta väärin"), inputFields[3]);
        } else {
            hideErrorMessage();

            atis.aseta_lahtokiitotie(inputFields[0]->text().toStdString());
            atis.aseta_laskukiitotie(inputFields[1]->text().toStdString());
            atis.aseta_siirtokorkeus(inputFields[2]->text().toInt());
            atis.aseta_siirtopinta(inputFields[3]->text().toInt());

            emit atisDone();
        }
    }

signals:
	void atisDone();

private:
    Metar& metar;
    Atis& atis;
    Peli& peli;

	QLabel* title;
    QLabel* inputLabel;
    QLabel* infoLabel;
    QLabel* metarLabel;
    QLabel* error;

	QPushButton* okButton;

    std::vector <QLineEdit*> inputFields;
    int level;
    bool ekaPiirto;

    QTimer* timer;
};

#endif
