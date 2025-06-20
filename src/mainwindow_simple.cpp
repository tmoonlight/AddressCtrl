#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Qt CMake 应用程序 - 使用封装的富文本编辑器");
    setMinimumSize(400, 300);
    resize(1200, 900);
    
    // 启用鼠标跟踪
    setMouseTracking(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // 创建中央控件
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建布局
    mainLayout = new QVBoxLayout(centralWidget);
    
    // 设置状态栏
    setupStatusBar();
    
    // 创建标题
    titleLabel = new QLabel("富文本编辑器 - Qt CMake应用程序!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin: 10px;");
    titleLabel->setMouseTracking(true);
    titleLabel->installEventFilter(this);
    
    infoLabel = new QLabel("下面是封装的富文本编辑器控件:", this);
    infoLabel->setMouseTracking(true);
    infoLabel->installEventFilter(this);
    
    // 创建富文本编辑器控件
    richTextEditor = new RichTextEditorWidget(this);
    
    // 创建退出按钮
    buttonLayout = new QHBoxLayout();
    exitButton = new QPushButton("退出", this);
    exitButton->setStyleSheet("QPushButton { padding: 8px 16px; border-radius: 4px; font-size: 12px; background-color: #95a5a6; color: white; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(exitButton);
    
    // 添加控件到布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(richTextEditor);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号和槽
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
    
    // 连接富文本编辑器的信号
    connect(richTextEditor, &RichTextEditorWidget::textChanged, [this]() {
        // 可以在这里处理文本变化事件
    });
    
    connect(richTextEditor, &RichTextEditorWidget::tagInserted, [this](const QString &tagText) {
        // 可以在这里处理标签插入事件
        qDebug() << "标签已插入:" << tagText;
    });
}

void MainWindow::setupStatusBar()
{
    // 创建状态栏
    mainStatusBar = this->statusBar();
    
    // 创建鼠标位置显示标签
    mousePositionLabel = new QLabel("鼠标位置: (0, 0)", this);
    mousePositionLabel->setStyleSheet("padding: 2px 10px; color: #555;");
    
    // 将标签添加到状态栏
    mainStatusBar->addPermanentWidget(mousePositionLabel);
    
    // 设置状态栏样式
    mainStatusBar->setStyleSheet("QStatusBar { border-top: 1px solid #cccccc; background-color: #f5f5f5; }");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 处理鼠标移动事件，实时更新鼠标位置
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint mainWindowPos;
        
        QWidget *widget = qobject_cast<QWidget*>(obj);
        if (widget) {
            QPoint widgetPos = mouseEvent->pos();
            QPoint globalPos = widget->mapToGlobal(widgetPos);
            mainWindowPos = this->mapFromGlobal(globalPos);
        } else {
            QPoint globalPos = mouseEvent->globalPos();
            mainWindowPos = this->mapFromGlobal(globalPos);
        }
        
        mousePositionLabel->setText(QString("鼠标位置: (%1, %2)").arg(mainWindowPos.x()).arg(mainWindowPos.y()));
    }
    
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 更新鼠标位置显示
    mousePositionLabel->setText(QString("鼠标位置: (%1, %2)").arg(event->pos().x()).arg(event->pos().y()));
    
    // 调用父类的事件处理
    QMainWindow::mouseMoveEvent(event);
}
