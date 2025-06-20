#include "richtexteditorwidget.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QFontDatabase>

RichTextEditorWidget::RichTextEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    // 初始化标签文本对象，并设置管理器为自己
    tagTextObject = new TagTextObject(this);
    
    setupUI();
}

RichTextEditorWidget::~RichTextEditorWidget()
{
    delete tagTextObject;
}

void RichTextEditorWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // 设置工具栏
    setupToolbar();
    mainLayout->addWidget(toolbar);
    
    // 创建输入区域
    inputLayout = new QHBoxLayout();
    
    inputLineEdit = new QLineEdit(this);
    inputLineEdit->setPlaceholderText("请输入要添加的文本...");
    inputLineEdit->setMouseTracking(true);
    inputLineEdit->installEventFilter(this);
    
    addButton = new QPushButton("添加文本", this);
    addButton->setEnabled(false);
    
    insertTagButton = new QPushButton("插入人员", this);
    insertTagButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 6px 12px; border-radius: 3px; }");
    
    inputLayout->addWidget(inputLineEdit);
    inputLayout->addWidget(addButton);
    inputLayout->addWidget(insertTagButton);
    
    mainLayout->addLayout(inputLayout);
    
    // 创建富文本编辑器
    textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText("这是一个富文本编辑器，您可以设置字体、颜色、样式等...");
    textEdit->setAcceptRichText(true);
    textEdit->setMouseTracking(true);
    
    // 注册自定义文本对象
    textEdit->document()->documentLayout()->registerHandler(TagTextObject::TagTextFormat, tagTextObject);
    
    // 为textEdit和其视口启用鼠标跟踪和事件过滤器
    textEdit->installEventFilter(this);
    textEdit->viewport()->setMouseTracking(true);
    textEdit->viewport()->installEventFilter(this);
    
    mainLayout->addWidget(textEdit);
    
    // 连接信号和槽
    connect(addButton, &QPushButton::clicked, this, &RichTextEditorWidget::onAddTextClicked);
    connect(insertTagButton, &QPushButton::clicked, this, &RichTextEditorWidget::insertPersonTag);
    connect(inputLineEdit, &QLineEdit::textChanged, this, &RichTextEditorWidget::onInputTextChanged);
    connect(inputLineEdit, &QLineEdit::returnPressed, this, &RichTextEditorWidget::onAddTextClicked);
      // 连接textEdit的文本变化信号
    connect(textEdit, &QTextEdit::textChanged, [this]() {
        clearAllTags();
        emit textChanged();
    });
}

void RichTextEditorWidget::setupToolbar()
{
    toolbar = new QToolBar("富文本工具栏", this);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    // 字体选择
    toolbar->addWidget(new QLabel("字体: "));
    fontComboBox = new QFontComboBox(this);
    fontComboBox->setFixedWidth(150);
    toolbar->addWidget(fontComboBox);
    connect(fontComboBox, &QFontComboBox::currentFontChanged, this, &RichTextEditorWidget::changeFont);
    
    toolbar->addSeparator();
    
    // 字体大小
    toolbar->addWidget(new QLabel("大小: "));
    fontSizeSpinBox = new QSpinBox(this);
    fontSizeSpinBox->setRange(8, 72);
    fontSizeSpinBox->setValue(12);
    fontSizeSpinBox->setFixedWidth(60);
    toolbar->addWidget(fontSizeSpinBox);
    connect(fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &RichTextEditorWidget::changeFontSize);
    
    toolbar->addSeparator();
    
    // 粗体
    boldAction = toolbar->addAction("粗体");
    boldAction->setCheckable(true);
    boldAction->setShortcut(QKeySequence::Bold);
    connect(boldAction, &QAction::triggered, this, &RichTextEditorWidget::toggleBold);
    
    // 斜体
    italicAction = toolbar->addAction("斜体");
    italicAction->setCheckable(true);
    italicAction->setShortcut(QKeySequence::Italic);
    connect(italicAction, &QAction::triggered, this, &RichTextEditorWidget::toggleItalic);
    
    // 下划线
    underlineAction = toolbar->addAction("下划线");
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(QKeySequence::Underline);
    connect(underlineAction, &QAction::triggered, this, &RichTextEditorWidget::toggleUnderline);
    
    toolbar->addSeparator();
    
    // 文字颜色
    colorAction = toolbar->addAction("文字颜色");
    connect(colorAction, &QAction::triggered, this, &RichTextEditorWidget::changeTextColor);
    
    // 背景颜色
    backgroundColorAction = toolbar->addAction("背景颜色");
    connect(backgroundColorAction, &QAction::triggered, this, &RichTextEditorWidget::changeBackgroundColor);
}

// 公共接口实现
QString RichTextEditorWidget::getPlainText() const
{
    return textEdit->toPlainText();
}

QString RichTextEditorWidget::getHtmlText() const
{
    return textEdit->toHtml();
}

void RichTextEditorWidget::setPlainText(const QString &text)
{
    textEdit->setPlainText(text);
}

void RichTextEditorWidget::setHtmlText(const QString &html)
{
    textEdit->setHtml(html);
}

void RichTextEditorWidget::clear()
{
    textEdit->clear();
    inputLineEdit->clear();
}

QTextEdit* RichTextEditorWidget::getTextEdit() const
{
    return textEdit;
}

// 事件过滤器
bool RichTextEditorWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        // 检查是否按下了分号键（支持全角和半角）
        if (keyEvent->text() == ";" || keyEvent->text() == "；") {
            QTextCursor cursor = textEdit->textCursor();
            QTextBlock currentBlock = cursor.block();
            QString blockText = currentBlock.text();
            
            // 找到光标位置在块中的位置
            int positionInBlock = cursor.positionInBlock();
            
            // 查找最后一个标签或行开始的位置
            int startPos = 0;
            
            // 遍历当前块，找到最后一个标签对象
            QTextBlock::iterator it;
            for (it = currentBlock.begin(); !it.atEnd(); ++it) {
                QTextFragment fragment = it.fragment();
                if (fragment.charFormat().objectType() == TagTextObject::TagTextFormat) {
                    // 找到标签对象，更新起始位置到标签之后
                    startPos = fragment.position() + fragment.length() - currentBlock.position();
                }
            }
            
            // 提取从最后一个标签到当前位置的文本
            QString textToConvert = blockText.mid(startPos, positionInBlock - startPos).trimmed();
            
            if (!textToConvert.isEmpty()) {
                // 删除要转换的文本
                cursor.setPosition(currentBlock.position() + startPos);
                cursor.setPosition(currentBlock.position() + positionInBlock, QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
                
                // 转换为人员标签
                convertTextToPersonTag(textToConvert);
                
                // 阻止分号的输入
                return true;
            }
        }
    }
    
    // 处理鼠标移动事件
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
          // 传递鼠标位置给TagTextObject（仅对textEdit的视口进行处理）
        if (obj == textEdit->viewport()) {
            QPoint viewportPos = mouseEvent->pos();
            tagTextObject->setCurrentMousePos(viewportPos);
              // 使用精确的矩形检测来判断是否悬停在TagTextObject上
            int hoveredTagPosition = getHoveredTagPosition(viewportPos);
            bool isOnTag = (hoveredTagPosition != -1);
            
            if (isOnTag) {
                // 鼠标在标签上，设置为手形指针
                textEdit->viewport()->setCursor(Qt::PointingHandCursor);
            } else {
                // 鼠标不在标签上，恢复默认指针
                textEdit->viewport()->setCursor(Qt::IBeamCursor);
            }
            
            textEdit->viewport()->update(); // 触发重绘以更新悬停效果
        }
    }
      // 处理鼠标离开事件，恢复默认鼠标指针
    if (obj == textEdit->viewport() && event->type() == QEvent::Leave) {
        textEdit->viewport()->setCursor(Qt::IBeamCursor);
        tagTextObject->setCurrentMousePos(QPoint(-1, -1)); // 清除鼠标位置
        textEdit->viewport()->update(); // 触发重绘以清除悬停效果
    }
    
    return QWidget::eventFilter(obj, event);
}

// 槽函数实现
void RichTextEditorWidget::onAddTextClicked()
{
    QString text = inputLineEdit->text().trimmed();
    if (!text.isEmpty()) {
        // 保存当前光标位置
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        
        // 添加带格式的文本
        cursor.insertText("• ");
        
        // 设置当前字体格式
        QTextCharFormat format;
        format.setFontFamily(fontComboBox->currentFont().family());
        format.setFontPointSize(fontSizeSpinBox->value());
        format.setFontWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
        format.setFontItalic(italicAction->isChecked());
        format.setFontUnderline(underlineAction->isChecked());
        
        cursor.insertText(text, format);
        cursor.insertText("\n");
        
        textEdit->setTextCursor(cursor);
        inputLineEdit->clear();
        inputLineEdit->setFocus();
    }
}

void RichTextEditorWidget::onInputTextChanged()
{
    bool hasText = !inputLineEdit->text().trimmed().isEmpty();
    addButton->setEnabled(hasText);
}

void RichTextEditorWidget::toggleBold()
{
    QTextCharFormat format;
    format.setFontWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void RichTextEditorWidget::toggleItalic()
{
    QTextCharFormat format;
    format.setFontItalic(italicAction->isChecked());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void RichTextEditorWidget::toggleUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline(underlineAction->isChecked());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void RichTextEditorWidget::changeFont()
{
    QTextCharFormat format;
    format.setFontFamily(fontComboBox->currentFont().family());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void RichTextEditorWidget::changeFontSize()
{
    QTextCharFormat format;
    format.setFontPointSize(fontSizeSpinBox->value());
    textEdit->mergeCurrentCharFormat(format);
    textEdit->setFocus();
}

void RichTextEditorWidget::changeTextColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择文字颜色");
    if (color.isValid()) {
        QTextCharFormat format;
        format.setForeground(color);
        textEdit->mergeCurrentCharFormat(format);
        textEdit->setFocus();
    }
}

void RichTextEditorWidget::changeBackgroundColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "选择背景颜色");
    if (color.isValid()) {
        QTextCharFormat format;
        format.setBackground(color);
        textEdit->mergeCurrentCharFormat(format);
        textEdit->setFocus();
    }
}

void RichTextEditorWidget::insertPersonTag()
{
    QString name = inputLineEdit->text().trimmed();
    
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
    
    // 插入标签对象
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);
    
    // 在标签后添加一个空格
    cursor.insertText(" ");
    
    textEdit->setTextCursor(cursor);
    
    // 清空输入框并重新聚焦
    inputLineEdit->clear();
    inputLineEdit->setFocus();
    
    emit tagInserted(name);
}

void RichTextEditorWidget::convertTextToPersonTag(const QString &text)
{
    QTextCursor cursor = textEdit->textCursor();
    
    // 创建自定义格式
    QTextCharFormat format;
    format.setObjectType(TagTextObject::TagTextFormat);
    format.setProperty(TagTextObject::TagProperty, text);
    
    // 插入标签对象
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);
    
    // 在标签后添加一个空格
    cursor.insertText(" ");
    
    textEdit->setTextCursor(cursor);
    
    emit tagInserted(text);
}

// 标签管理方法实现
void RichTextEditorWidget::updateTagRect(int position, const QRectF &rect, const QString &text)
{
    // 查找是否已存在该位置的标签
    for (int i = 0; i < allTags.size(); ++i) {
        if (allTags[i].position == position) {
            // 更新现有标签
            allTags[i].rect = rect;
            allTags[i].text = text;
            return;
        }
    }
    
    // 添加新标签
    allTags.append(TagInfo(rect, position, text));
}

int RichTextEditorWidget::getHoveredTagPosition(const QPoint &mousePos) const
{
    // 遍历所有标签，找到鼠标悬停的标签
    for (const TagInfo &tag : allTags) {
        // 调整矩形检测，考虑左右3像素的留白
        QRectF adjustedRect = tag.rect.adjusted(3, 0, -3, 0);
        if (adjustedRect.contains(mousePos)) {
            return tag.position;
        }
    }
    return -1; // 没有找到悬停的标签
}

void RichTextEditorWidget::clearAllTags()
{
    allTags.clear();
}

#include "richtexteditorwidget.moc"
