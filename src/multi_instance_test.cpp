#include "richtexteditorwidget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建主窗口
    QWidget mainWindow;
    mainWindow.setWindowTitle("多实例富文本编辑器测试");
    mainWindow.resize(1200, 600);

    QHBoxLayout *layout = new QHBoxLayout(&mainWindow);

    // 创建第一个富文本编辑器
    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->addWidget(new QLabel("编辑器 1"));
    RichTextEditorWidget *editor1 = new RichTextEditorWidget();
    leftLayout->addWidget(editor1);

    // 创建第二个富文本编辑器
    QWidget *rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->addWidget(new QLabel("编辑器 2"));
    RichTextEditorWidget *editor2 = new RichTextEditorWidget();
    rightLayout->addWidget(editor2);

    layout->addWidget(leftWidget);
    layout->addWidget(rightWidget);

    // 为每个编辑器设置一些示例文本
    editor1->setPlainText("这是第一个编辑器，请输入一些文本然后用分号结尾测试标签功能；");
    editor2->setPlainText("这是第二个编辑器，也可以独立创建标签；");

    mainWindow.show();

    return app.exec();
}
