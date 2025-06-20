#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QColorDialog>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化标签文本对象
    tagTextObject = new TagTextObject();
      setupUI();
    setWindowTitle("Qt CMake 应用程序");
    setMinimumSize(400, 300);
    resize(1200, 900);
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
    buttonLayout = new QHBoxLayout();
    
    // 创建富文本工具栏
    setupRichTextToolbar();
    
    // 创建控件
    titleLabel = new QLabel("富文本编辑器 - Qt CMake应用程序!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin: 10px;");
    
    infoLabel = new QLabel("在下面的富文本框中输入和编辑内容:", this);
    
    lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText("请输入要添加的文本...");
    
    textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText("这是一个富文本编辑器，您可以设置字体、颜色、样式等...");
    textEdit->setAcceptRichText(true);  // 启用富文本支持
    
    // 注册自定义文本对象
    textEdit->document()->documentLayout()->registerHandler(TagTextObject::TagTextFormat, tagTextObject);
    
    clickButton = new QPushButton("添加文本", this);
    clearButton = new QPushButton("清空", this);
    exitButton = new QPushButton("退出", this);
    
    // 设置按钮样式
    QString buttonStyle = "QPushButton { padding: 8px 16px; border-radius: 4px; font-size: 12px; } "
                         "QPushButton:hover { background-color: #e0e0e0; }";
    clickButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #3498db; color: white; }");
    clearButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #e74c3c; color: white; }");
    exitButton->setStyleSheet(buttonStyle + "QPushButton { background-color: #95a5a6; color: white; }");
    
    // 添加控件到布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(richTextToolbar);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(textEdit);
    
    buttonLayout->addWidget(clickButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(exitButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // 连接信号和槽
    connect(clickButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(clearButton, &QPushButton::clicked, textEdit, &QTextEdit::clear);
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
    connect(lineEdit, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(lineEdit, &QLineEdit::returnPressed, this, &MainWindow::onButtonClicked);
}

void MainWindow::setupRichTextToolbar()
{
    richTextToolbar = new QToolBar("富文本工具栏", this);
    richTextToolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    // 字体选择
    richTextToolbar->addWidget(new QLabel("字体: "));
    fontComboBox = new QFontComboBox(this);
    fontComboBox->setFixedWidth(150);
    richTextToolbar->addWidget(fontComboBox);
    connect(fontComboBox, &QFontComboBox::currentFontChanged, this, &MainWindow::changeFont);
    
    richTextToolbar->addSeparator();
    
    // 字体大小
    richTextToolbar->addWidget(new QLabel("大小: "));
    fontSizeSpinBox = new QSpinBox(this);
    fontSizeSpinBox->setRange(8, 72);
    fontSizeSpinBox->setValue(12);
    fontSizeSpinBox->setFixedWidth(60);
    richTextToolbar->addWidget(fontSizeSpinBox);
    connect(fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeFontSize);
    
    richTextToolbar->addSeparator();
    
    // 粗体
    boldAction = richTextToolbar->addAction("粗体");
    boldAction->setCheckable(true);
    boldAction->setShortcut(QKeySequence::Bold);
    connect(boldAction, &QAction::triggered, this, &MainWindow::toggleBold);
    
    // 斜体
    italicAction = richTextToolbar->addAction("斜体");
    italicAction->setCheckable(true);
    italicAction->setShortcut(QKeySequence::Italic);
    connect(italicAction, &QAction::triggered, this, &MainWindow::toggleItalic);
    
    // 下划线
    underlineAction = richTextToolbar->addAction("下划线");
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(QKeySequence::Underline);
    connect(underlineAction, &QAction::triggered, this, &MainWindow::toggleUnderline);
    
    richTextToolbar->addSeparator();
    
    // 文字颜色
    colorAction = richTextToolbar->addAction("文字颜色");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeTextColor);
    
    // 背景颜色
    backgroundColorAction = richTextToolbar->addAction("背景颜色");
    connect(backgroundColorAction, &QAction::triggered, this, &MainWindow::changeBackgroundColor);
    
    richTextToolbar->addSeparator();
    
    // 插入人员标签按钮
    insertTagButton = new QPushButton("插入人员", this);
    insertTagButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 6px 12px; border-radius: 3px; }");
    richTextToolbar->addWidget(insertTagButton);
    connect(insertTagButton, &QPushButton::clicked, this, &MainWindow::insertPersonTag);
}

void MainWindow::onButtonClicked()
{
    QString text = lineEdit->text().trimmed();
    if (!text.isEmpty()) {
        // 保存当前光标位置
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        
        // 添加带格式的文本
        cursor.insertText("• ");
        
        // 设置当前字体格式
        QTextCharFormat format;
        //*format.setAnchorNames(QString("test"));
        format.setFontFamily(fontComboBox->currentFont().family());
        format.setFontPointSize(fontSizeSpinBox->value());
        format.setFontWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
        format.setFontItalic(italicAction->isChecked());
        format.setFontUnderline(underlineAction->isChecked());
        
        cursor.insertText(text, format);
        cursor.insertText("\n");
        
        textEdit->setTextCursor(cursor);
        lineEdit->clear();
        lineEdit->setFocus();
    } else {
        QMessageBox::information(this, "提示", "请先输入一些文本!");
    }
}

void MainWindow::onTextChanged()
{
    bool hasText = !lineEdit->text().trimmed().isEmpty();
    clickButton->setEnabled(hasText);
}

void MainWindow::toggleBold()
{
    QTextCharFormat format;
    format.setFontWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void MainWindow::toggleItalic()
{
    QTextCharFormat format;
    format.setFontItalic(italicAction->isChecked());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void MainWindow::toggleUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline(underlineAction->isChecked());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void MainWindow::changeFont()
{
    QTextCharFormat format;
    format.setFontFamily(fontComboBox->currentFont().family());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void MainWindow::changeFontSize()
{
    QTextCharFormat format;
    format.setFontPointSize(fontSizeSpinBox->value());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void MainWindow::changeColor()
{
}

void MainWindow::changeTextColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择文字颜色");
    if (color.isValid()) {
        QTextCharFormat format;
        format.setForeground(color);
        textEdit->mergeCurrentCharFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::changeBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择背景颜色");
    if (color.isValid()) {
        QTextCharFormat format;
        format.setBackground(color);
        textEdit->mergeCurrentCharFormat(format);
        textEdit->setFocus();
    }
}

void MainWindow::insertPersonTag()
{
    QString name = lineEdit->text().trimmed();
    
    // 如果输入框为空，使用默认标签文本
    if (name.isEmpty()) {
        name = "默认人员";
    }
    
    // 获取当前光标位置
    QTextCursor cursor = textEdit->textCursor();
    
    // 创建自定义格式
    QTextCharFormat format;
    format.setObjectType(TagTextObject::TagTextFormat);
    format.setProperty(TagTextObject::TagProperty, name);
    
    // 插入一个特殊字符作为对象占位符
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);
    
    // 在标签后添加一个空格，便于继续输入
    cursor.insertText(" ");
    
    textEdit->setTextCursor(cursor);
    
    // 清空输入框并重新聚焦
    lineEdit->clear();
    lineEdit->setFocus();
}
