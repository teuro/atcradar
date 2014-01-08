#ifndef _LEVELMENU_H_
#define _LEVELMENU_H_

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class LevelMenu : public QWidget
{
	Q_OBJECT
public:
    LevelMenu(QWidget* parent = 0) : QWidget(parent) {
		slider = new QSlider(Qt::Horizontal, this);
		slider->setGeometry(50, 50, 130, 30);
        slider->setMinimum(0);
        slider->setMaximum(4);

		title = new QLabel("Valitse vaikeustaso: ", this);
		title->setGeometry(50, 20, 150, 30);

		levelLabel = new QLabel("0", this);
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
		emit levelSelected(2);
		//close();
	}

signals:
	void levelSelected(int level);

private:
	QLabel* title;
	QSlider* slider;
	QLabel* levelLabel;
	QPushButton* okButton;
};

#endif
