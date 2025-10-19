#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (MainWindow::getInstance() == nullptr)
        qFatal() << "Process: main window invalid";
    // 打开开始界面关卡
    MainWindow::getInstance()->openStartMenuLevel();
    return a.exec();
}
