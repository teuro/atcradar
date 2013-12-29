#include <QApplication>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QPushButton>

#include "levelmenu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LevelMenu window;

    window.resize(250, 150);
    window.setWindowTitle("Simple example");
    window.show();

    return app.exec();
}
