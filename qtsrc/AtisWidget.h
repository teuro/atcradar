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
        emit atisDone();

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
};

#endif
