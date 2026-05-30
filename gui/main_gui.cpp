#include <QApplication>
#include <QIcon>

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QIcon appIcon(":/assets/logo.png");
    if (!appIcon.isNull())
    {
        app.setWindowIcon(appIcon);
    }
    MainWindow window;
    window.show();
    return app.exec();
}
