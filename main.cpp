#include "inttoolgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IntToolGUI w;
    w.show();
    return a.exec();
}
