#include "gui/MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordList wordList;
    MainWindow w(wordList);
    w.show();
    return a.exec();
}


