#ifndef MAINWINDOW_SIMPLE_H
#define MAINWINDOW_SIMPLE_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QMouseEvent>
#include "richtexteditorwidget.h"

class MainWindowSimple : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowSimple(QWidget *parent = nullptr);
    ~MainWindowSimple();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void setupStatusBar();
    
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    
    QLabel *titleLabel;
    QLabel *infoLabel;
    QPushButton *exitButton;
    
    // 富文本编辑器控件
    RichTextEditorWidget *richTextEditor;
    
    // 状态栏
    QStatusBar *mainStatusBar;
    QLabel *mousePositionLabel;
};

#endif // MAINWINDOW_SIMPLE_H
