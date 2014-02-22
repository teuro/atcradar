#ifndef _LEVELMENU_H_
#define _LEVELMENU_H_

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QDir>
#include <QComboBox>
#include <QString>
#include <QDebug>
#include <QTableWidget>
#include <QStringList>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "apuvalineet.hpp"

class LevelMenu : public QWidget {
	Q_OBJECT
    QTableWidget* tilastot;
    QStringList otsikot;
public:
    LevelMenu(QWidget* parent = 0) : QWidget(parent) {
		slider = new QSlider(Qt::Horizontal, this);
		slider->setGeometry(50, 50, 130, 30);
        slider->setMinimum(1);
        slider->setMaximum(4);

        tilastot = new QTableWidget(this);
        tilastot->move(300, 30);
        tilastot->setColumnCount(2);
        tilastot->setRowCount(4);
        otsikot << tr("Taso") << tr("Pisteet");
        tilastot->setHorizontalHeaderLabels(otsikot);
        tilastot->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tilastot->setSelectionBehavior(QAbstractItemView::SelectRows);
        tilastot->setSelectionMode(QAbstractItemView::SingleSelection);
        tilastot->setShowGrid(true);
        tilastot->setStyleSheet("QTableView {selection-background-color: red;}");

        std::ifstream sisaan("data/pisteet.txt", std::ios::in);

        if (!sisaan) {
            throw std::runtime_error(tr("Tiedosto data/pisteet.txt ei aukea tai se puuttuu").toStdString());
        }

        std::vector <std::string> kohdat;
        std::string rivi;
        std::map <int, int> pisteet;

        while (std::getline(sisaan, rivi)) {
            kohdat = apuvalineet::pilko_rivi(rivi, "|");

            pisteet[apuvalineet::luvuksi<int>(kohdat[0])] = apuvalineet::luvuksi<int>(kohdat[1]);
        }

        for (std::map <int, int> ::iterator it = pisteet.begin(); it != pisteet.end(); ++it) {
            tilastot->setItem(it->first-1, 0, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(it->first))));
            tilastot->setItem(it->first-1, 1, new QTableWidgetItem(QString::fromStdString(apuvalineet::tekstiksi(it->second))));
        }

        title = new QLabel(tr("Valitse vaikeustaso:"), this);
        title->setGeometry(50, 20, 150, 30);

        levelLabel = new QLabel(QString::fromStdString(apuvalineet::tekstiksi(slider->minimum())) , this);
        levelLabel->setGeometry(230, 50, 20, 30);

        polku = new QDir("kentat/");
        tiedostot = new QStringList(polku->entryList(QDir::Files));
        valinta = new QComboBox(this);
        valinta->setGeometry(50, 100, 100, 20);
        valinta->addItems(*tiedostot);
        valinta->show();

        okButton = new QPushButton(tr("OK"), this);
        okButton->move(50, 140);

        connect(slider, SIGNAL(valueChanged(int)), levelLabel, SLOT(setNum(int)));
        connect(okButton, SIGNAL(clicked()), this, SLOT(kun_ok_painettu()));
	}

    virtual ~LevelMenu() {
		delete slider;
		delete title;
		delete levelLabel;
		delete okButton;
	}

public slots:
    void kun_ok_painettu() {
        emit taso_valittu(slider->value(), valinta->currentText().toStdString());
		//close();
    }

signals:
    void taso_valittu(int level, std::string kentta);

private:
	QLabel* title;
	QSlider* slider;
	QLabel* levelLabel;
    QLabel* ruutu;
    QPushButton* okButton;
    QDir* polku;
    QStringList* tiedostot;
    QComboBox* valinta;
    QPushButton* suomi;
    QPushButton* englanti;
};

#endif
