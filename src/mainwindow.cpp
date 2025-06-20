#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Qt CMake ???? - ????????");
    setMinimumSize(600, 400);
    resize(1200, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // ??????
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // ????
    mainLayout = new QVBoxLayout(centralWidget);
    
    // ????
    titleLabel = new QLabel("???????? - Qt CMake????!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");
      infoLabel = new QLabel("??????????????????????????:", this);
    infoLabel->setStyleSheet("color: #7f8c8d; margin: 5px;");
    
    // ???????
    editorsLayout = new QHBoxLayout();
    
    // ?????????????
    QWidget *editor1Container = new QWidget();
    QVBoxLayout *editor1Layout = new QVBoxLayout(editor1Container);
    QLabel *editor1Label = new QLabel("??? 1", this);
    editor1Label->setStyleSheet("font-weight: bold; margin: 5px;");
    richTextEditor1 = new RichTextEditorWidget(this);
    editor1Layout->addWidget(editor1Label);
    editor1Layout->addWidget(richTextEditor1);
    
    // ?????????????
    QWidget *editor2Container = new QWidget();
    QVBoxLayout *editor2Layout = new QVBoxLayout(editor2Container);
    QLabel *editor2Label = new QLabel("??? 2", this);
    editor2Label->setStyleSheet("font-weight: bold; margin: 5px;");
    richTextEditor2 = new RichTextEditorWidget(this);
    editor2Layout->addWidget(editor2Label);
    editor2Layout->addWidget(richTextEditor2);
    
    // ??????????
    editorsLayout->addWidget(editor1Container);
    editorsLayout->addWidget(editor2Container);
    
    // ??????
    buttonLayout = new QHBoxLayout();
    
    clearButton = new QPushButton("????", this);
    clearButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    exitButton = new QPushButton("??", this);
    exitButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(exitButton);
      // ??????
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addLayout(editorsLayout);
    mainLayout->addLayout(buttonLayout);
    
    // ??????
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(exitButton, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    connect(richTextEditor1, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditor1TextChanged);
    connect(richTextEditor2, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditor2TextChanged);
    
    // ????????
    richTextEditor1->setPlainText("??????????1?????????????????????????????");
    richTextEditor2->setPlainText("??????????????????????");
}

void MainWindow::onButtonClicked()
{
    richTextEditor1->clear();
    richTextEditor2->clear();
}

void MainWindow::onEditor1TextChanged()
{
    // ??????????1?????????
}

void MainWindow::onEditor2TextChanged()
{
    // ??????????2?????????
}
