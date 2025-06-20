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
#include "tagtextobject.h"

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
    void onTextChanged();
    void toggleBold();
    void toggleItalic();
    void toggleUnderline();
    void changeFont();
    void changeFontSize();
    void changeColor();
    void changeTextColor();        // 新增
    void changeBackgroundColor();  // 新增
    void insertPersonTag();        // 新增：插入人员标签

private:
    void setupUI();
    void setupRichTextToolbar();
    
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QToolBar *richTextToolbar;
    
    QLabel *titleLabel;
    QLabel *infoLabel;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QPushButton *clickButton;
    QPushButton *clearButton;
    QPushButton *exitButton;
    
    // 富文本控件
    QAction *boldAction;
    QAction *italicAction;
    QAction *underlineAction;
    QAction *colorAction;
    QAction *backgroundColorAction;
    QFontComboBox *fontComboBox;
    QSpinBox *fontSizeSpinBox;
    QPushButton *insertTagButton; // 新增：插入标签按钮
    
    // 标签文本对象
    TagTextObject *tagTextObject;
};

#endif // MAINWINDOW_H
