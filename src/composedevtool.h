#ifndef COMPOSEDEVTOOL_H
#define COMPOSEDEVTOOL_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include <QLoggingCategory>
#include <QMessageLogContext>
#include <QMutex>
#include <QScrollBar>

class ComposeDevTool : public QWidget
{
    Q_OBJECT

public:
    explicit ComposeDevTool(QWidget *parent = nullptr);
    ~ComposeDevTool();

    // 静态方法用于作为消息处理器
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    
    // 单例访问
    static ComposeDevTool* getInstance();
    
    // 显示/隐藏工具窗口
    void toggleVisibility();

public slots:
    void clearLogs();
    void saveLogsToFile();
    void appendLogMessage(QtMsgType type, const QString &message, const QString &context = QString());

private:
    void setupUI();
    void setupMessageHandler();
    QString formatLogMessage(QtMsgType type, const QString &message, const QString &context);
    QColor getColorForMessageType(QtMsgType type);
    
    // UI 组件
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QTextEdit *logTextEdit;
    QPushButton *clearButton;
    QPushButton *saveButton;
    QPushButton *closeButton;
    QLabel *statusLabel;
    
    // 静态实例
    static ComposeDevTool* instance;
    static QMutex instanceMutex;
    
    // 消息缓冲
    QMutex logMutex;
    int maxLogLines;
    
    // 原始消息处理器
    static QtMessageHandler originalHandler;
};

#endif // COMPOSEDEVTOOL_H
