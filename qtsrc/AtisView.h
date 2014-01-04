#ifndef _ATISVIEW_H_
#define _ATISVIEW_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>

class AtisView : public QWidget
{
	Q_OBJECT
public:
    AtisView(QWidget* parent = 0) : QWidget(parent) {
		title = new QLabel("ATIS Valinta", this);
        title->setGeometry(0, 0, 150, 30);

		okButton = new QPushButton("OK", this);
		okButton->move(80, 85);

        departRWY = new QLineEdit("", this);
        departRWY->move(120, 120);

        departure = new QLabel("Lähtöbaana", this);
        departure->setGeometry(50, 120, 60, 10);

        landingRWY = new QLineEdit("", this);
        landingRWY->move(120, 140);

        landing = new QLabel("Laskubaana", this);
        landing->setGeometry(50, 140, 60, 10);

        TL = new QLineEdit("", this);
        TL->move(120, 160);

        transitionLevel = new QLabel("Siirtokorkeus", this);
        transitionLevel->setGeometry(50, 160, 60, 10);

        TA = new QLineEdit("", this);
        TA->move(120, 180);

        transitionAltitude = new QLabel("Siirtopinta", this);
        transitionAltitude->setGeometry(50, 180, 60, 10);

        connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
        connect(departRWY, SIGNAL(textChanged(const QString &text)), this, SLOT(setText(const QString &text)));
        connect(landingRWY, SIGNAL(textChanged(const QString& text)), this, SLOT(setText(const QString& text)));
        connect(TL, SIGNAL(textChanged(const QString& text)), this, SLOT(setText(const QString& text)));
        connect(TA, SIGNAL(textChanged(const QString& text)), this, SLOT(setText(const QString& text)));
	}

	public slots:
    void OnOkPressed() {
		emit atisDone();
		//close();
	}

    void lineChanged( ) {}

signals:
	void atisDone();

private:
	QLabel* title;
    QLabel* departure;
    QLabel* landing;
    QLabel* transitionAltitude;
    QLabel* transitionLevel;

	QPushButton* okButton;

    QLineEdit* departRWY;
    QLineEdit* landingRWY;
    QLineEdit* TA;
    QLineEdit* TL;
};

#endif
