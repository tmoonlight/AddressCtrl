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

private:
    void setupUI();    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *editorsLayout;  // 用于放置两个编辑器的水平布局
    
    QLabel *titleLabel;
    QLabel *infoLabel;
    QPushButton *clearButton;
    QPushButton *exitButton;
    
    // 使用富文本编辑器控件
    RichTextEditorWidget *richTextEditor1;
    RichTextEditorWidget *richTextEditor2;
};

#endif // MAINWINDOW_H
