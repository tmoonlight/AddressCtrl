#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Qt dsfa adsf asdf 演示");
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
    titleLabel = new QLabel("富文本编辑器演示 - Qt C123123Make应用程序!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");
      infoLabel = new QLabel("使用下面的富文本编辑器，支持字体格式、人员标签等功能:", this);
    infoLabel->setStyleSheet("color: #7f8c8d; margin: 5px;");
    
    // 创建编辑器布局
    editorsLayout = new QHBoxLayout();
    
    // 创建第一个富文本编辑器控件
    QWidget *editor1Container = new QWidget();
    QVBoxLayout *editor1Layout = new QVBoxLayout(editor1Container);
    QLabel *editor1Label = new QLabel("编辑器 1", this);
    editor1Label->setStyleSheet("font-weight: bold; margin: 5px;");
    richTextEditor1 = new RichTextEditorWidget(this);
    editor1Layout->addWidget(editor1Label);
    editor1Layout->addWidget(richTextEditor1);
    
    // 创建第二个富文本编辑器控件
    QWidget *editor2Container = new QWidget();
    QVBoxLayout *editor2Layout = new QVBoxLayout(editor2Container);
    QLabel *editor2Label = new QLabel("编辑器 2", this);
    editor2Label->setStyleSheet("font-weight: bold; margin: 5px;");
    richTextEditor2 = new RichTextEditorWidget(this);
    editor2Layout->addWidget(editor2Label);
    editor2Layout->addWidget(richTextEditor2);
    
    // 添加编辑器到水平布局
    editorsLayout->addWidget(editor1Container);
    editorsLayout->addWidget(editor2Container);
    
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
    mainLayout->addLayout(editorsLayout);
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号和槽
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(exitButton, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    connect(richTextEditor1, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditor1TextChanged);
    connect(richTextEditor2, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditor2TextChanged);
    
    // 设置一些示例文本
    richTextEditor1->setPlainText("欢迎使用富文本编辑器1！您可以输入文本，然后在文本后添加分号将其转换为人员标签；");
    richTextEditor2->setPlainText("这是第二个富文本编辑器；支持独立的标签管理；");
}

void MainWindow::onButtonClicked()
{
    richTextEditor1->clear();
    richTextEditor2->clear();
}

void MainWindow::onEditor1TextChanged()
{
    // 可以在这里添加编辑器1文本变化的处理逻辑
}

void MainWindow::onEditor2TextChanged()
{
    // 可以在这里添加编辑器2文本变化的处理逻辑
}
