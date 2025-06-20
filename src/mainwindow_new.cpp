#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Qt CMake 应用程序 - 富文本编辑器演示");
    setMinimumSize(600, 400);
    resize(1200, 600);
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
    
    // 创建标题
    titleLabel = new QLabel("富文本编辑器演示 - Qt CMake应用程序!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");
    
    infoLabel = new QLabel("使用下面的富文本编辑器，支持字体格式、人员标签等功能:", this);
    infoLabel->setStyleSheet("color: #7f8c8d; margin: 5px;");
    
    // 创建富文本编辑器控件
    richTextEditor = new RichTextEditorWidget(this);
    
    // 创建按钮布局
    buttonLayout = new QHBoxLayout();
    
    clearButton = new QPushButton("清空内容", this);
    clearButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    exitButton = new QPushButton("退出", this);
    exitButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(exitButton);
    
    // 添加到主布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(richTextEditor);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号和槽
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(exitButton, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    connect(richTextEditor, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditorTextChanged);
    
    // 设置一些示例文本
    richTextEditor->setPlainText("欢迎使用富文本编辑器！您可以输入文本，然后在文本后添加分号将其转换为人员标签；");
}

void MainWindow::onButtonClicked()
{
    richTextEditor->clear();
}

void MainWindow::onEditorTextChanged()
{
    // 可以在这里添加文本变化的处理逻辑
}
