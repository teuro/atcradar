#ifndef _ATISVIEW_H_
#define _ATISVIEW_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QValidator>

class AtisView : public QWidget
{
	Q_OBJECT
public:
    AtisView(QWidget* parent = 0) : QWidget(parent) {
		title = new QLabel("ATIS Valinta", this);
        title->setGeometry(0, 0, 150, 30);

        Metar = new QLabel("EFRO 1320z 13025KT 080OVC", this);
        Metar->setGeometry(0, 0, 600, 10);

		okButton = new QPushButton("OK", this);
		okButton->move(80, 85);

        QValidator* validate_dep = new QIntValidator(01, 35, this);
        QValidator* validate_ldg = new QIntValidator(01, 35, this);
        QValidator* validate_TL = new QIntValidator(40, 70, this);
        QValidator* validate_TA = new QIntValidator(3000, 18000, this);

        departRWY = new QLineEdit("", this);
        departRWY->move(120, 120);
        departRWY->setValidator(validate_dep);

        departure = new QLabel("Lähtöbaana", this);
        departure->setGeometry(50, 120, 60, 10);

        landingRWY = new QLineEdit("", this);
        landingRWY->move(120, 140);
        landingRWY->setValidator(validate_ldg);

        landing = new QLabel("Laskubaana", this);
        landing->setGeometry(50, 140, 60, 10);

        TA = new QLineEdit("", this);
        TA->move(120, 160);
        TA->setValidator(validate_TA);

        transitionLevel = new QLabel("Siirtokorkeus", this);
        transitionLevel->setGeometry(50, 160, 60, 10);

        TL = new QLineEdit("", this);
        TL->move(120, 180);
        TL->setValidator(validate_TL);

        transitionAltitude = new QLabel("Siirtopinta", this);
        transitionAltitude->setGeometry(50, 180, 60, 10);

        connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
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
    QLabel* departure;
    QLabel* landing;
    QLabel* transitionAltitude;
    QLabel* transitionLevel;
    QLabel* Metar;

	QPushButton* okButton;

    QLineEdit* departRWY;
    QLineEdit* landingRWY;
    QLineEdit* TA;
    QLineEdit* TL;
};

#endif
