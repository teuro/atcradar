#ifndef _ATISVIEW_H_
#define _ATISVIEW_H_

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class AtisView : public QWidget
{
	Q_OBJECT
public:
	AtisView(QWidget* parent = 0) : QWidget(parent)
	{
		title = new QLabel("ATIS Valinta", this);
		title->setGeometry(50, 20, 150, 30);

		okButton = new QPushButton("OK", this);
		okButton->move(80, 85);

		connect(okButton, SIGNAL(clicked()), this, SLOT(OnOkPressed()));
	}

	public slots:
	void OnOkPressed()
	{
		emit atisDone();
		//close();
	}

signals:
	void atisDone();

private:
	QLabel* title;
	QPushButton* okButton;
};

#endif