#include "widget.h"
#include "Wizbz_Keyboard_Security.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    AntiDebug ad{};

    ad.Run();

    w.show();
    return a.exec();
}
