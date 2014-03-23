#ifndef PELAAJAWIDGET_HPP
#define PELAAJAWIDGET_HPP

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QLineEdit>
#include <QString>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "apuvalineet.hpp"
#include "peli.hpp"
#include "pelisuorite.hpp"

class Pelaajawidget : public QWidget {
    Q_OBJECT
    Peli& peli;
public:
    Pelaajawidget(Peli& p, QWidget* parent = 0) : peli(p), QWidget(parent) {
        title = new QLabel(tr("Valitse tunnus:"), this);
        title->setGeometry(50, 20, 150, 30);

        tunnus = new QLineEdit("", this);
        tunnus->move(50, 50);

        okButton = new QPushButton(tr("OK"), this);
        okButton->move(50, 80);

        std::vector <std::string> rivit = apuvalineet::lue_tiedosto("data/pisteet.txt");
        std::vector <std::string> solut;
        std::clog << rivit.size() << std::endl;

        int y = 50;

        std::vector <pelisuorite> pistevektori;

        for (unsigned int i = 0; i < rivit.size(); ++i) {
            solut = apuvalineet::pilko_rivi(rivit[i], "|");
            int id          = apuvalineet::luvuksi<int>(solut[0]);
            int taso        = apuvalineet::luvuksi<int>(solut[1]);
            int koneita     = apuvalineet::luvuksi<int>(solut[2]);
            int porrastus   = apuvalineet::luvuksi<int>(solut[3]);
            int muut        = apuvalineet::luvuksi<int>(solut[4]);
            int pisteet     = apuvalineet::luvuksi<int>(solut[5]);
            int tarkiste    = apuvalineet::luvuksi<int>(solut[6]);

            pistevektori.push_back(pelisuorite(id, taso, koneita, porrastus, muut, pisteet));

            if (pistevektori.back().tarkista(tarkiste)) {
                QLabel* pistetunnus = new QLabel(QString::fromStdString(peli.pelaajat[id].anna_tunnus()), this);
                pistetunnus->move(220, y);

                QLabel* pistearvo = new QLabel(QString::fromStdString(apuvalineet::tekstiksi(pisteet)), this);
                pistearvo->move(270, y);

                y += 20;
            } else {
                std::clog << solut[6] << " != " << pistevektori.back().laske_tiiviste() << std::endl;
            }
        }

        connect(okButton, SIGNAL(clicked()), this, SLOT(kun_ok_painettu()));
    }

    virtual ~Pelaajawidget() {
        delete title;
        delete okButton;
    }

public slots:
    void kun_ok_painettu() {
        emit pelaaja_valittu(tunnus->text().toStdString());
        //close();
    }

signals:
    void pelaaja_valittu(std::string);

private:
    QLabel* title;
    QPushButton* okButton;
    QLineEdit* tunnus;
};

#endif // PELAAJAWIDGET_HPP
