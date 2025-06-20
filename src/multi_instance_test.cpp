#include "richtexteditorwidget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ?????
    QWidget mainWindow;
    mainWindow.setWindowTitle("???????????");
    mainWindow.resize(1200, 600);

    QHBoxLayout *layout = new QHBoxLayout(&mainWindow);

    // ???????????
    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->addWidget(new QLabel("??? 1"));
    RichTextEditorWidget *editor1 = new RichTextEditorWidget();
    leftLayout->addWidget(editor1);

    // ???????????
    QWidget *rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->addWidget(new QLabel("??? 2"));
    RichTextEditorWidget *editor2 = new RichTextEditorWidget();
    rightLayout->addWidget(editor2);

    layout->addWidget(leftWidget);
    layout->addWidget(rightWidget);

    // ??????????????
    editor1->setPlainText("??????????????????????????????");
    editor2->setPlainText("???????????????????");

    mainWindow.show();

    return app.exec();
}
