#include <QApplication>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QPushButton>

class LevelMenu : public QWidget
{
	Q_OBJECT
public:
	LevelMenu()
	{
		slider = new QSlider(Qt::Horizontal, this);
		slider->setGeometry(50, 50, 130, 30);

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
		close();
	}

private:
	QLabel* title;
	QSlider* slider;
	QLabel* levelLabel;
	QPushButton* okButton;
};

