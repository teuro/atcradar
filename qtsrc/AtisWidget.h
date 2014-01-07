#ifndef _ATISVIEW_H_
#define _ATISVIEW_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QValidator>
#include <iostream>
#include <fstream>
#include <vector>
#include "Metar.hpp"
#include "../src/apuvalineet.hpp"

class paineraja {
public:
    int alaraja;
    int ylaraja;
    int siirtopinta;

    paineraja (int ar, int yr, int sp) {
        this->alaraja = ar;
        this->ylaraja = yr;
        this->siirtopinta = sp;
    }
};

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

	public slots:
    void OnOkPressed() {
        //std::ofstream ulos("C:/Users/acer/Documents/GitHub/atcradar/ohjelma/data/tulos.txt");
        downloadPrressureLimit("C:/Users/acer/Documents/GitHub/atcradar/ohjelma/data/painerajat.txt", inputFields[2]->text().toInt());

        if (calculateTL(metar.anna_paine()) == inputFields[3]->text().toInt()) {
            emit atisDone();
        } else {
            //ulos << calculateTL(metar.anna_paine()) << " " << inputFields[3]->text().toInt() << std::endl;
        }

		//close();
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
    std::vector <paineraja> painerajat;

    void downloadPrressureLimit(std::string file, int siirtokorkeus) {
        std::ifstream in(file.c_str(), std::ios::in);

        if (!in) {
            throw std::runtime_error("File " + file + " cannot be open");
        }

        std::string line;
        std::vector <std::string> words;
        int place;

        while (std::getline(in, line)) {
            words = apuvalineet::pilko_rivi(line, "|");

            if (words[0] == "P" && apuvalineet::luvuksi<int>(words[1]) == siirtokorkeus) {
                place = apuvalineet::luvuksi<int>(words[2]);
            } else if (words[0] == "K") {
                painerajat.push_back(paineraja(apuvalineet::luvuksi<int>(words[1]), apuvalineet::luvuksi<int>(words[2]), apuvalineet::luvuksi<int>(words[place])));
            }

            words.clear();
        }

        in.close();
    }

    int calculateTL(int pressure) {
        //std::ofstream ulos("C:/Users/acer/Documents/GitHub/atcradar/ohjelma/data/tulos.txt");
        for (unsigned int i = 0; i < painerajat.size(); ++i) {
            //ulos << painerajat[i].alaraja << " " << painerajat[i].ylaraja << " " << painerajat[i].siirtopinta << std::endl;
            if (pressure >= painerajat[i].alaraja && pressure <= painerajat[i].ylaraja) {
                return painerajat[i].siirtopinta;
            }
        }
    }
};

#endif
