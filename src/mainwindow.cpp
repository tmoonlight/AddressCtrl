#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

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
    
    // 添加测试控件到第一个编辑器容器（用于测试Tab键焦点切换）
    QLineEdit *testInput1 = new QLineEdit(this);
    testInput1->setPlaceholderText("测试输入框1 - 用于验证Tab键切换");
    //testInput1->setStyleSheet("margin: 5px; padding: 5px;");
    editor1Layout->addWidget(testInput1);
    
    // 创建第二个富文本编辑器控件
    QWidget *editor2Container = new QWidget();
    QVBoxLayout *editor2Layout = new QVBoxLayout(editor2Container);
    QLabel *editor2Label = new QLabel("编辑器 2", this);
    editor2Label->setStyleSheet("font-weight: bold; margin: 5px;");
    richTextEditor2 = new RichTextEditorWidget(this);
    editor2Layout->addWidget(editor2Label);
    editor2Layout->addWidget(richTextEditor2);
    
    // 添加测试控件到第二个编辑器容器
    QLineEdit *testInput2 = new QLineEdit(this);
    testInput2->setPlaceholderText("测试输入框2 - 用于验证Tab键切换");
    //testInput2->setStyleSheet("margin: 5px; padding: 5px;");
    editor2Layout->addWidget(testInput2);
    
    // 添加编辑器到水平布局
    editorsLayout->addWidget(editor1Container);
    editorsLayout->addWidget(editor2Container);
    
    // 创建按钮布局
    buttonLayout = new QHBoxLayout();
      clearButton = new QPushButton("清空内容", this);
    clearButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    showTextButton = new QPushButton("显示完整文本", this);
    showTextButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    lineHeightButton = new QPushButton("测试行高", this);
    lineHeightButton->setStyleSheet("QPushButton { background-color: #f39c12; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    triggerTagButton = new QPushButton("创建标签", this);
    triggerTagButton->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; padding: 8px 16px; border-radius: 4px; }");
      triggerErrorTagButton = new QPushButton("创建错误标签", this);
    triggerErrorTagButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    themeColorButton = new QPushButton("更换主题色", this);
    themeColorButton->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    exitButton = new QPushButton("退出", this);
    exitButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border-radius: 4px; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(showTextButton);
    buttonLayout->addWidget(lineHeightButton);
    buttonLayout->addWidget(triggerTagButton);
    buttonLayout->addWidget(triggerErrorTagButton);
    buttonLayout->addWidget(themeColorButton);
    buttonLayout->addWidget(exitButton);
      // 添加到主布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addLayout(editorsLayout);
    mainLayout->addLayout(buttonLayout);
      // 连接信号和槽
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(showTextButton, &QPushButton::clicked, this, &MainWindow::showCompleteText);
    connect(lineHeightButton, &QPushButton::clicked, this, &MainWindow::testLineHeight);    connect(triggerTagButton, &QPushButton::clicked, this, &MainWindow::triggerTagCreation);
    connect(triggerErrorTagButton, &QPushButton::clicked, this, &MainWindow::triggerErrorTagCreation);
    connect(themeColorButton, &QPushButton::clicked, this, &MainWindow::changeThemeColor);
    connect(exitButton, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
    connect(richTextEditor1, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditor1TextChanged);
    connect(richTextEditor2, &RichTextEditorWidget::textChanged, this, &MainWindow::onEditor2TextChanged);
    
    // 连接新的textChangedFromLastTag信号
    connect(richTextEditor1, &RichTextEditorWidget::textChangedFromLastTag, 
            this, &MainWindow::onEditor1TextChangedFromLastTag);
    connect(richTextEditor2, &RichTextEditorWidget::textChangedFromLastTag, 
            this, &MainWindow::onEditor2TextChangedFromLastTag);
    
    // 设置一些示例文本
    //richTextEditor1->setPlainText("欢迎使用富文本编辑器1！您可以输入文本，然后在文本后添加分号将其转换为人员标签；");
    //richTextEditor2->setPlainText("这是第二个富文本编辑器；支持独立的标签管理；");
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

void MainWindow::showCompleteText()
{
    QString completeText1 = richTextEditor1->getCompleteText(true);
    QString completeText2 = richTextEditor2->getCompleteText();
    
    QString message = QString("编辑器1的完整文本:\n%1\n\n编辑器2的完整文本:\n%2")
                      .arg(completeText1.isEmpty() ? "(空)" : completeText1)
                      .arg(completeText2.isEmpty() ? "(空)" : completeText2);
    
    QMessageBox::information(this, "完整文本内容", message);
}

void MainWindow::onEditor1TextChangedFromLastTag(const QString &text)
{
    qDebug() << "编辑器1 - 从上一个tag到光标的文本:" << text;
}

void MainWindow::onEditor2TextChangedFromLastTag(const QString &text)
{
    qDebug() << "编辑器2 - 从上一个tag到光标的文本:" << text;
}

void MainWindow::testLineHeight()
{
    static int testHeight = 20;  // 静态变量用于循环测试不同的行高
    
    // 循环测试不同的行高：20, 30, 40, 然后回到默认
    if (testHeight == 20) {
        testHeight = 30;
    } else if (testHeight == 30) {
        testHeight = 40;
    } else if (testHeight == 40) {
        testHeight = 24; // 默认行高
    } else {
        testHeight = 20;
    }
    
    // 应用到两个编辑器
    richTextEditor1->setLineHeight(testHeight);
    richTextEditor2->setLineHeight(testHeight);
    
    qDebug() << "行高已设置为:" << testHeight << "像素";
    
    // 显示信息
    QString message = QString("行高已设置为: %1 像素\n\n"
                             "编辑器1当前行高: %2 像素\n"
                             "编辑器2当前行高: %3 像素")
                             .arg(testHeight)
                             .arg(richTextEditor1->getLineHeight())
                             .arg(richTextEditor2->getLineHeight());
    
    QMessageBox::information(this, "行高测试", message);
}

void MainWindow::triggerTagCreation()
{
    // 获取当前具有焦点的编辑器
    RichTextEditorWidget* activeEditor = nullptr;
    
    if (richTextEditor1->getTextEdit()->hasFocus()) {
        activeEditor = richTextEditor1;
    } else if (richTextEditor2->getTextEdit()->hasFocus()) {
        activeEditor = richTextEditor2;
    } else {
        // 如果没有编辑器有焦点，默认使用第一个编辑器
        activeEditor = richTextEditor1;
        activeEditor->getTextEdit()->setFocus();
    }
    
    // 触发标签创建
    activeEditor->triggerTagCreation();
    
    qDebug() << "手动触发标签创建完成";
}

void MainWindow::triggerErrorTagCreation()
{
    // 获取当前具有焦点的编辑器
    RichTextEditorWidget* activeEditor = nullptr;
    
    if (richTextEditor1->getTextEdit()->hasFocus()) {
        activeEditor = richTextEditor1;
    } else if (richTextEditor2->getTextEdit()->hasFocus()) {
        activeEditor = richTextEditor2;
    } else {
        // 如果没有编辑器有焦点，默认使用第一个编辑器
        activeEditor = richTextEditor1;
        activeEditor->getTextEdit()->setFocus();
    }
    
    // 触发错误标签创建
    activeEditor->triggerTagCreation(TagType::Error);
    
    qDebug() << "手动触发错误标签创建完成";
}

void MainWindow::changeThemeColor()
{
    // 定义几种预设的主题色
    static QList<QColor> themeColors = {
        QColor(70, 130, 180),   // 钢蓝色（默认）
        QColor(52, 152, 219),   // 亮蓝色
        QColor(155, 89, 182),   // 紫色
        QColor(46, 204, 113),   // 绿色
        QColor(231, 76, 60),    // 红色
        QColor(230, 126, 34),   // 橙色
        QColor(241, 196, 15)    // 黄色
    };
    
    static int currentIndex = 0;
    currentIndex = (currentIndex + 1) % themeColors.size();
    
    QColor newColor = themeColors[currentIndex];
    
    // 为两个编辑器设置新的主题色
    richTextEditor1->setThemeColor(newColor);
    richTextEditor2->setThemeColor(newColor);
    
    // 更新按钮文本显示当前颜色
    themeColorButton->setText(QString("主题色: %1").arg(newColor.name()));
    
    QMessageBox::information(this, "主题色已更改", 
                           QString("新的主题色: %1\n\n现在鼠标悬停或点击文本框时边框会变成这个颜色！")
                           .arg(newColor.name()));
}
