#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QFontComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextObjectInterface>
#include <QStatusBar>
#include <QMouseEvent>
#include "tagtextobject.h"
#include "richtexteditorwidget.h"

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();
    void onEditor1TextChanged();
    void onEditor2TextChanged();
    void showCompleteText(); // 新增：显示完整文本内容
    void changeThemeColor(); // 新增：更改主题色

    void onEditor1TextChangedFromLastTag(const QString &text); // 新增：编辑器1从上一个tag到光标的文本变化
    void onEditor2TextChangedFromLastTag(const QString &text); // 新增：编辑器2从上一个tag到光标的文本变化
    void testLineHeight(); // 新增：测试行高设置
    void triggerTagCreation(); // 新增：触发标签创建
    void triggerErrorTagCreation(); // 新增：触发错误标签创建

private:
    void setupUI();    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *editorsLayout;  // 用于放置两个编辑器的水平布局
      QLabel *titleLabel;
    QLabel *infoLabel;
    QPushButton *clearButton;
    QPushButton *showTextButton; // 新增：显示完整文本按钮
    QPushButton *lineHeightButton; // 新增：行高测试按钮
    QPushButton *triggerTagButton; // 新增：触发标签创建按钮
    QPushButton *triggerErrorTagButton; // 新增：触发错误标签创建按钮
    QPushButton *themeColorButton; // 新增：主题色测试按钮
    QPushButton *exitButton;
    
    // 使用富文本编辑器控件
    RichTextEditorWidget *richTextEditor1;
    RichTextEditorWidget *richTextEditor2;
};

#endif // MAINWINDOW_H
