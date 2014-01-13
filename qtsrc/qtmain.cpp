#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <qdesktopwidget.h>
#include <QRect>
#include <QDebug>
#include <time.h>
#include "mainwindow.h"

int main(int argc, char** argv) {
	srand(time(NULL));

    try {
        QApplication app(argc, argv);

        Kieli kieli("fi_FI");
        MainWindow window(kieli);

        window.resize(800, 750);
        window.setWindowTitle("ATC RADAR");
        window.show();

        return app.exec();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
