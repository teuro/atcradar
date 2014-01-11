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

        metarLabel = new QLabel(m.getMessage(), this);
        metarLabel->setGeometry(0, 0, 600, 10);

        addInputField("Lähtökiitotie", 100, 60, 1, 36);
        addInputField("Laskukiitotie", 100, 80, 1, 36);
        addInputField("Siirtokorkeus", 100, 100, 3000, 18000);
        addInputField("Siirtopinta", 100, 120, 30, 220);

		okButton = new QPushButton("OK", this);
        okButton->move(100, 160);

        connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
	}

    void addInputField(QString name, int x, int y, int minimum, int maximum) {
        inputFields.push_back(new QLineEdit("", this));
        inputFields.back()->move(x, y);

        QIntValidator* tmp = new QIntValidator(minimum, maximum, inputFields.back());
        inputFields.back()->setValidator(tmp);

        inputLabel = new QLabel(name, this);
        inputLabel->setGeometry(x-80, y-0, 200, 20);
    }

    void drawErrorMessage(std::string info, QLineEdit* field) {
        QToolTip::showText(field->mapToGlobal(QPoint()), QString::fromStdString(info));
    }

    void hideErrorMessage() {
        QToolTip::hideText();
    }

    public slots:
    void OnOkPressed() {
        atis.tyhjenna();
        atis.downloadPrressureLimit("data/painerajat.txt", inputFields[2]->text().toInt());

        std::clog << peli.kentta.kiitotiet.size() << std::endl;

        std::vector <kiitotie> :: iterator haku_lahto = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), inputFields[0]->text().toStdString());
        std::vector <kiitotie> :: iterator haku_lasku = std::find(peli.kentta.kiitotiet.begin(), peli.kentta.kiitotiet.end(), inputFields[1]->text().toStdString());

        double vasta_lahto = apuvalineet::laske_vastatuuli(inputFields[0]->text().toInt() * 10, metar.anna_tuuli());
        double vasta_lasku = apuvalineet::laske_vastatuuli(inputFields[1]->text().toInt() * 10, metar.anna_tuuli());
        int laskettu_siirtopinta = atis.calculateTL(metar.anna_paine());

        std::clog << apuvalineet::laske_vastatuuli((inputFields[0]->text().toInt() * 10), metar.anna_tuuli()) << std::endl;

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
    QLabel* metarLabel;
    QLabel* error;

	QPushButton* okButton;

    std::vector <QLineEdit*> inputFields;
};

#endif
