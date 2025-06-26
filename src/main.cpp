#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 输出一些测试信息来演示不同类型的调试输出
    qDebug() << "Application starting...";
    qInfo() << "Debug tool initialized";
    qWarning() << "This is a test warning message";
    
    MainWindow window;
    window.show();
    
    qDebug() << "Main window displayed";
    qInfo() << "Press Alt+] to open debug console";
    
    return app.exec();
}