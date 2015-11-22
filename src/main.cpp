#include <QApplication>

#include "inc/controldialog.h"

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    ControlDialog control;
    control.show();

    return app.exec();
}
