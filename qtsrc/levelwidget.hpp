#ifndef _LEVELMENU_H_
#define _LEVELMENU_H_

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QString>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "apuvalineet.hpp"

class LevelMenu : public QWidget
{
	Q_OBJECT
public:
    LevelMenu(QWidget* parent = 0) : QWidget(parent) {
		slider = new QSlider(Qt::Horizontal, this);
		slider->setGeometry(50, 50, 130, 30);
        slider->setMinimum(1);
        slider->setMaximum(4);

		title = new QLabel("Valitse vaikeustaso: ", this);
        title->setGeometry(50, 20, 150, 30);

        levelLabel = new QLabel(QString::fromStdString(apuvalineet::tekstiksi(slider->minimum())) , this);
        levelLabel->setGeometry(230, 50, 20, 30);

		okButton = new QPushButton("OK", this);
		okButton->move(80, 85);

        connect(slider, SIGNAL(valueChanged(int)), levelLabel, SLOT(setNum(int)));
		connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
	}

	virtual ~LevelMenu()
	{
		delete slider;
		delete title;
		delete levelLabel;
		delete okButton;
	}

public slots:
    void OnOkPressed()
    {
        emit levelSelected(slider->value());
		//close();
    }

signals:
	void levelSelected(int level);

private:
	QLabel* title;
	QSlider* slider;
	QLabel* levelLabel;
    QLabel* ruutu;
    QPushButton* okButton;

    QString getInfo(int level) {
        std::ifstream sisaan("data/tasot.txt", std::ios::in);

        if (!sisaan) {
            throw std::runtime_error("Tiedosto data/tasot.txt ei aukea");
        }

        std::string teksti;

        while (std::getline(sisaan, teksti)) {
            std::vector <std::string> asiat = apuvalineet::pilko_rivi(teksti, "|");
            if (apuvalineet::luvuksi<int>(asiat[0]) == level) {
                std::clog << asiat[1] << std::endl;
                sisaan.close();
                return QString::fromStdString(asiat[1]);
            }
        }
    }
};

#endif
