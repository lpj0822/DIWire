#pragma execution_character_set("utf-8")
#include "dialog.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("DIWire");
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    Dialog w;
    w.show();

    return a.exec();
}
