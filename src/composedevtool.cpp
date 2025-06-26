#include "composedevtool.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QFont>
#include <QFontMetrics>

// 静态成员初始化
ComposeDevTool* ComposeDevTool::instance = nullptr;
QMutex ComposeDevTool::instanceMutex;
QtMessageHandler ComposeDevTool::originalHandler = nullptr;

ComposeDevTool::ComposeDevTool(QWidget *parent)
    : QWidget(parent), maxLogLines(10000)
{
    setupUI();
    setupMessageHandler();
    
    // 设置窗口属性
    setWindowTitle("Compose Dev Tool - Debug Console");
    setWindowIcon(QIcon(":/icons/debug.png")); // 如果有图标的话
    resize(800, 600);
    
    // 设置窗口标志，使其保持在最前面
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
}

ComposeDevTool::~ComposeDevTool()
{
    // 恢复原始消息处理器
    if (originalHandler) {
        qInstallMessageHandler(originalHandler);
    }
}

ComposeDevTool* ComposeDevTool::getInstance()
{
    QMutexLocker locker(&instanceMutex);
    if (!instance) {
        instance = new ComposeDevTool();
    }
    return instance;
}

void ComposeDevTool::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // 状态标签
    statusLabel = new QLabel("Debug Console - Ready", this);
    statusLabel->setStyleSheet("QLabel { background-color: #2c3e50; color: white; padding: 5px; font-weight: bold; }");
    
    // 日志显示区域
    logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    logTextEdit->setFont(QFont("Consolas", 9)); // 使用等宽字体
    logTextEdit->setStyleSheet(
        "QTextEdit {"
        "   background-color: #1e1e1e;"
        "   color: #d4d4d4;"
        "   border: 1px solid #3c3c3c;"
        "   selection-background-color: #264f78;"
        "}"
    );
    
    // 按钮布局
    buttonLayout = new QHBoxLayout();
    
    clearButton = new QPushButton("Clear Logs", this);
    clearButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #dc3545;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c82333;"
        "}"
    );
    
    saveButton = new QPushButton("Save Logs", this);
    saveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #28a745;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #218838;"
        "}"
    );
    
    closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #6c757d;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5a6268;"
        "}"
    );
    
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    // 添加到主布局
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(logTextEdit);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号
    connect(clearButton, &QPushButton::clicked, this, &ComposeDevTool::clearLogs);
    connect(saveButton, &QPushButton::clicked, this, &ComposeDevTool::saveLogsToFile);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::hide);
}

void ComposeDevTool::setupMessageHandler()
{
    // 保存原始消息处理器
    originalHandler = qInstallMessageHandler(nullptr);
    
    // 安装我们的消息处理器
    qInstallMessageHandler(ComposeDevTool::messageHandler);
}

void ComposeDevTool::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 调用原始处理器（如果存在）
    if (originalHandler) {
        originalHandler(type, context, msg);
    }
    
    // 发送到我们的工具窗口
    ComposeDevTool* tool = getInstance();
    if (tool) {
        QString contextInfo = QString("%1:%2").arg(context.file ? context.file : "Unknown").arg(context.line);
        QMetaObject::invokeMethod(tool, "appendLogMessage", Qt::QueuedConnection,
                                Q_ARG(QtMsgType, type),
                                Q_ARG(QString, msg),
                                Q_ARG(QString, contextInfo));
    }
}

QString ComposeDevTool::formatLogMessage(QtMsgType type, const QString &message, const QString &context)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString typeStr;
    
    switch (type) {
    case QtDebugMsg:    typeStr = "[DEBUG]"; break;
    case QtInfoMsg:     typeStr = "[INFO]"; break;
    case QtWarningMsg:  typeStr = "[WARN]"; break;
    case QtCriticalMsg: typeStr = "[ERROR]"; break;
    case QtFatalMsg:    typeStr = "[FATAL]"; break;
    }
    
    QString formatted = QString("[%1] %2 %3").arg(timestamp, typeStr, message);
    if (!context.isEmpty() && context != "Unknown:0") {
        formatted += QString(" (%1)").arg(context);
    }
    
    return formatted;
}

QColor ComposeDevTool::getColorForMessageType(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:    return QColor("#569cd6");  // 蓝色 - Debug
    case QtInfoMsg:     return QColor("#4ec9b0");  // 青色 - Info
    case QtWarningMsg:  return QColor("#dcdcaa");  // 黄色 - Warning
    case QtCriticalMsg: return QColor("#f44747");  // 红色 - Error
    case QtFatalMsg:    return QColor("#ff6b6b");  // 亮红色 - Fatal
    default:            return QColor("#d4d4d4");  // 默认白色
    }
}

void ComposeDevTool::appendLogMessage(QtMsgType type, const QString &message, const QString &context)
{
    QMutexLocker locker(&logMutex);
    
    // 格式化消息
    QString formattedMessage = formatLogMessage(type, message, context);
    
    // 获取对应颜色
    QColor color = getColorForMessageType(type);
    
    // 在 UI 线程中更新显示
    QTextCursor cursor = logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    
    // 设置文本格式
    QTextCharFormat format;
    format.setForeground(color);
    
    cursor.setCharFormat(format);
    cursor.insertText(formattedMessage + "\n");
    
    // 限制最大行数
    QTextDocument *doc = logTextEdit->document();
    if (doc->blockCount() > maxLogLines) {
        cursor.movePosition(QTextCursor::Start);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    }
    
    // 自动滚动到底部
    QScrollBar *scrollBar = logTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
    
    // 更新状态
    static int messageCount = 0;
    messageCount++;
    statusLabel->setText(QString("Debug Console - %1 messages").arg(messageCount));
}

void ComposeDevTool::clearLogs()
{
    logTextEdit->clear();
    statusLabel->setText("Debug Console - Cleared");
}

void ComposeDevTool::saveLogsToFile()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Debug Logs",
        QString("debug_logs_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        "Text Files (*.txt);;All Files (*)"
    );
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << logTextEdit->toPlainText();
            file.close();
            
            QMessageBox::information(this, "Success", "Logs saved successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save logs to file!");
        }
    }
}

void ComposeDevTool::toggleVisibility()
{
    if (isVisible()) {
        hide();
    } else {
        show();
        raise();
        activateWindow();
    }
}
