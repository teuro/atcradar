#ifndef _ATISVIEW_H_
#define _ATISVIEW_H_

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

class AtisView : public QWidget {
	Q_OBJECT
public:
    AtisView(Metar& m, QWidget* parent = 0) : QWidget(parent) {
        metar = m;
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

    void drawTooltip(std::string info, QLineEdit* field) {
        QToolTip::showText(field->mapToGlobal(QPoint()), QString::fromStdString(info));
    }

	public slots:
    void OnOkPressed() {
        Atis::downloadPrressureLimit("C:/Users/acer/Documents/GitHub/atcradar/ohjelma/data/painerajat.txt", inputFields[2]->text().toInt());

        double vasta_lahto = apuvalineet::laske_vastatuuli(inputFields[0]->text().toInt() * 10, metar.anna_tuuli());
        double vasta_lasku = apuvalineet::laske_vastatuuli(inputFields[1]->text().toInt() * 10, metar.anna_tuuli());
        int laskettu_siirtopinta = Atis::calculateTL(metar.anna_paine());

        if (laskettu_siirtopinta != inputFields[3]->text().toInt()) {
            drawTooltip(std::string("Laskepa uudestaan..."), inputFields[3]);
        } else if (vasta_lahto >= 0) {
            drawTooltip("Ei osunut...", inputFields[1]);
        } else if (vasta_lasku >= 0) {
            drawTooltip("Ei osunut...", inputFields[2]);
        } else {
            QToolTip::hideText();
            emit atisDone();
        }

	}

signals:
	void atisDone();

private:
    Metar metar;
	QLabel* title;
    QLabel* inputLabel;
    QLabel* metarLabel;
    QLabel* error;

	QPushButton* okButton;

    std::vector <QLineEdit*> inputFields;
};

#endif
