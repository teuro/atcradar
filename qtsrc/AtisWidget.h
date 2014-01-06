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
#include "Metar.hpp"
#include "../src/apuvalineet.hpp"

class AtisView : public QWidget {
	Q_OBJECT
public:
    AtisView(Metar& metar, QWidget* parent = 0) : QWidget(parent) {
		title = new QLabel("ATIS Valinta", this);
        title->setGeometry(0, 20, 150, 30);

        metarLabel = new QLabel(metar.getMessage(), this);
        metarLabel->setGeometry(0, 0, 600, 10);

        drawInputField("Lähtökiitotie", 100, 60, 1, 36);
        drawInputField("Laskukiitotie", 100, 80, 1, 36);
        drawInputField("Siirtokorkeus", 100, 100, 3000, 18000);
        drawInputField("Siirtopinta", 100, 120, 30, 220);

		okButton = new QPushButton("OK", this);
        okButton->move(100, 160);

        connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
	}

    void drawInputField(QString name, int x, int y, int minimum, int maximum) {
        inputField = new QLineEdit("", this);
        inputField->move(x, y);

        QIntValidator* tmp = new QIntValidator(minimum, maximum, inputField);
        inputField->setValidator(tmp);

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

	QLabel* title;
    QLabel* inputLabel;
    QLabel* metarLabel;

	QPushButton* okButton;

    QLineEdit* inputField;
};

#endif
