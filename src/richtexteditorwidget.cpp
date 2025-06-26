#include "richtexteditorwidget.h"
#include <QFontMetrics>
#include <QRegularExpression>
#include <QTextBlock>

RichTextEditorWidget::RichTextEditorWidget(QWidget *parent)
    : QWidget(parent), minimumHeight(0), maximumHeight(300), currentLineHeight(DEFAULT_LINE_HEIGHT),
      themeColor(QColor(70, 130, 180)), currentBorderColor(QColor(204, 204, 204)) // 默认主题色为钢蓝色，默认边框色为灰色
{
    // 初始化标签文本对象，并设置管理器为自己
    tagTextObject = new TagTextObject(this);
    
    // 初始化边框颜色动画
    borderAnimation = new QPropertyAnimation(this, "borderColor");
    borderAnimation->setDuration(300); // 300ms过渡时间
    borderAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    setupUI();
}

RichTextEditorWidget::~RichTextEditorWidget()
{
    delete tagTextObject;
}

void RichTextEditorWidget::setupUI()
{    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
      // 创建富文本编辑器
    textEdit = new QTextEdit(this);

    textEdit->setAcceptRichText(true);
    textEdit->setMouseTracking(true);
    textEdit->setFocusPolicy(Qt::StrongFocus); // 确保可以获取焦点
    
    // 设置滚动条策略
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);    // 设置文档的边距（影响文本内容与边框的距离）
    textEdit->document()->setDocumentMargin(0);

    // 检查文档的根框架边距
    QTextFrameFormat frameFormat = textEdit->document()->rootFrame()->frameFormat();
    frameFormat.setMargin(0);
    frameFormat.setTopMargin(-3);
    frameFormat.setBottomMargin(0);
    textEdit->document()->rootFrame()->setFrameFormat(frameFormat);

    // 初始化样式表
    updateStyleSheets();

    // 计算初始最小高度
    QFontMetrics fm(textEdit->font());
    int lineHeight = fm.lineSpacing();
    minimumHeight = lineHeight + textEdit->contentsMargins().top() + textEdit->contentsMargins().bottom() + 10;
    textEdit->setMinimumHeight(minimumHeight);
    textEdit->setMaximumHeight(maximumHeight);
    
    // 注册自定义文本对象
    textEdit->document()->documentLayout()->registerHandler(TagTextObject::TagTextFormat, tagTextObject);
    
    // 设置默认字符格式为垂直居中对齐
    QTextCharFormat defaultCharFormat;
    defaultCharFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
    textEdit->setCurrentCharFormat(defaultCharFormat);
    
    // 为textEdit和其视口启用鼠标跟踪和事件过滤器
    textEdit->installEventFilter(this);
    textEdit->viewport()->setMouseTracking(true);
    textEdit->viewport()->installEventFilter(this);
      mainLayout->addWidget(textEdit);
      // 连接信号
    connect(textEdit, &QTextEdit::textChanged, [this]() {
        clearAllTags();
        processDocumentForTags(); // 扫描文档并处理分号
        emit textChanged();
        emitTextChangedFromLastTag(); // 发射新的事件
    });
    
    // 连接文档内容变化信号来自动调整高度
    connect(textEdit->document(), &QTextDocument::contentsChanged, 
            this, &RichTextEditorWidget::adjustTextEditHeight);
    
    // 初始化高度
    adjustTextEditHeight();
    
    // 应用默认行高设置
    applyLineHeightToDocument();
    
    // 连接光标位置变化信号，确保新输入的文本也是垂直居中的
    connect(textEdit, &QTextEdit::cursorPositionChanged, [this]() {
        QTextCursor cursor = textEdit->textCursor();
        QTextCharFormat format = cursor.charFormat();
        if (format.verticalAlignment() != QTextCharFormat::AlignMiddle) {
            format.setVerticalAlignment(QTextCharFormat::AlignMiddle);
            cursor.setCharFormat(format);
            textEdit->setTextCursor(cursor);
        }
    });
}

// 公共接口
QString RichTextEditorWidget::getPlainText() const
{
    return textEdit->toPlainText();
}

QString RichTextEditorWidget::getHtmlText() const
{
    return textEdit->toHtml();
}

QString RichTextEditorWidget::getCompleteText(bool isTagOnly) const
{
    QString completeText;
    QTextDocument *doc = textEdit->document();
    
    // 遍历整个文档的所有块
    QTextBlock block = doc->begin();
    while (block.isValid()) {
        QTextBlock::iterator it;
        for (it = block.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            bool isTag = fragment.charFormat().objectType() == TagTextObject::TagTextFormat;
            if (isTag) {
                // 获取标签的文本内容
                QString tagText = fragment.charFormat().property(TagTextObject::TagProperty).toString();
                completeText += tagText;
            } else if (!isTagOnly) {
                // 普通文本片段，仅当非tagOnly时添加
                completeText += fragment.text();
            }
        }
        
        block = block.next();
        if (block.isValid() && !isTagOnly) {
            // 只在完整模式下添加换行符
            completeText += "\n";
        }
    }
    
    return completeText;
}

void RichTextEditorWidget::setPlainText(const QString &text)
{
    textEdit->setPlainText(text);
    applyLineHeightToDocument();
}

void RichTextEditorWidget::setHtmlText(const QString &html)
{
    textEdit->setHtml(html);
    applyLineHeightToDocument();
}

void RichTextEditorWidget::clear()
{
    textEdit->clear();
}

QTextEdit* RichTextEditorWidget::getTextEdit() const
{
    return textEdit;
}

// 高度控制接口
void RichTextEditorWidget::setMaximumTextHeight(int maxHeight)
{
    maximumHeight = maxHeight;
    textEdit->setMaximumHeight(maxHeight);
    adjustTextEditHeight();
}

int RichTextEditorWidget::getMaximumTextHeight() const
{
    return maximumHeight;
}

void RichTextEditorWidget::adjustTextEditHeight()
{
    // 获取文档的理想高度
    QTextDocument *doc = textEdit->document();
    int documentHeight = doc->size().height();
    
    // 计算单行文本的高度
    QFontMetrics fm(textEdit->font());
    int singleLineHeight = fm.height() + 13; // 添加一些边距
    
    // 设置最小高度为单行高度
    int newHeight = qMax(singleLineHeight, documentHeight);
    
    // 如果设置了最大高度，则限制高度
    if (maximumHeight > 0) {
        newHeight = qMin(newHeight, maximumHeight);
    }
    
    newHeight = newHeight+10;
    // 设置新的高度
    textEdit->setFixedHeight(newHeight);
    
    // 更新整个widget的高度
    updateGeometry();
}

// 事件过滤器
bool RichTextEditorWidget::eventFilter(QObject *obj, QEvent *event)
{    // 处理焦点事件
    if (obj == textEdit) {
        if (event->type() == QEvent::FocusIn) {
            animateBorderColor(themeColor);
            return false; // 继续传播事件
        } else if (event->type() == QEvent::FocusOut) {
            animateBorderColor(QColor(204, 204, 204)); // 恢复默认灰色
            return false; // 继续传播事件
        }
    }
    
    // 处理鼠标进入和离开事件
    if (obj == textEdit) {
        if (event->type() == QEvent::Enter) {
            if (!textEdit->hasFocus()) {
                animateBorderColor(themeColor);
            }
            return false; // 继续传播事件
        } else if (event->type() == QEvent::Leave) {
            if (!textEdit->hasFocus()) {
                animateBorderColor(QColor(204, 204, 204)); // 恢复默认灰色
            }
            return false; // 继续传播事件
        }
    }
    // 处理键盘事件
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        if (obj == textEdit && keyEvent->key() == Qt::Key_Tab) {
            // Tab键处理：跳转到下一个控件
            focusNextChild();
            return true; // 阻止事件传播，不让Tab字符插入到文本中
        }
        
        if (obj == textEdit && keyEvent->key() == Qt::Key_Backtab) {
            // Shift+Tab处理：跳转到上一个控件
            focusPreviousChild();
            return true; // 阻止事件传播
        }
    }
    
    // 处理鼠标点击事件
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
          if (obj == textEdit->viewport() && mouseEvent->button() == Qt::LeftButton) {
            QPoint viewportPos = mouseEvent->pos();
            
            // 检查是否点击了删除按钮
            for (const auto &tag : allTags) {
                if (tag.deleteRect.contains(viewportPos)) {
                    deleteTagAtPosition(tag.position);
                    return true; // 阻止事件传播
                }
            }
            
            // 检查是否点击了标签本身（但不是删除按钮）
            for (const auto &tag : allTags) {
                if (tag.rect.contains(viewportPos) && !tag.deleteRect.contains(viewportPos)) {
                    handleTagClick(viewportPos);
                    return true; // 阻止事件传播
                }
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
            bool isOnDeleteButton = isPointOnDeleteButton(viewportPos);
            
            if (isOnDeleteButton) {
                // 鼠标在删除按钮上，设置为箭头指针
                textEdit->viewport()->setCursor(Qt::WaitCursor);
            } else if (isOnTag) {
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

void RichTextEditorWidget::convertTextToPersonTag(const QString &text, TagType tagType)
{
    QTextCursor cursor = textEdit->textCursor();
    
    // 保存当前块的格式
    QTextBlockFormat currentBlockFormat = cursor.blockFormat();
    
    // 创建自定义对象格式
    QTextCharFormat tagFormat;
    tagFormat.setObjectType(TagTextObject::TagTextFormat);
    tagFormat.setProperty(TagTextObject::TagProperty, text);
    tagFormat.setProperty(TagTextObject::TagTypeProperty, static_cast<int>(tagType)); // 设置标签类型
    tagFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle); // 设置垂直居中对齐
    
    // 在光标位置插入标签对象
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), tagFormat);
    
    // 确保当前块保持行高设置
    QTextBlockFormat blockFormat = currentBlockFormat;
    blockFormat.setLineHeight(currentLineHeight * LINE_SPACING_FACTOR, QTextBlockFormat::FixedHeight);
    cursor.setBlockFormat(blockFormat);
    
    emit tagInserted(text);
}

// 标签管理接口
void RichTextEditorWidget::updateTagRect(int position, const QRectF &rect, const QString &text)
{
    // 更新现有标签的矩形
    for (auto &tag : allTags) {
        if (tag.position == position) {
            tag.rect = rect;
            tag.text = text;
            return;
        }
    }
    
    // 添加新的标签信息
    allTags.append(TagInfo(rect, position, text));
}

void RichTextEditorWidget::updateTagDeleteButtonRect(int position, const QRectF &deleteRect)
{
    // 更新对应标签的删除按钮矩形
    for (auto &tag : allTags) {
        if (tag.position == position) {
            tag.deleteRect = deleteRect;
            return;
        }
    }
}

int RichTextEditorWidget::getHoveredTagPosition(const QPoint &mousePos) const
{
    for (const auto &tag : allTags) {
        if (tag.rect.contains(mousePos)) {
            return tag.position;
        }
    }
    return -1; // 没有找到标签
}

void RichTextEditorWidget::clearAllTags()
{
    allTags.clear();
}

bool RichTextEditorWidget::isPointOnDeleteButton(const QPoint &mousePos) const
{
    for (const auto &tag : allTags) {
        if (tag.deleteRect.contains(mousePos)) {
            return true;
        }
    }
    return false;
}

void RichTextEditorWidget::deleteTagAtPosition(int position)
{
    QTextCursor cursor = textEdit->textCursor();
    cursor.setPosition(position);
    
    // 确保我们在正确的位置
    QTextCharFormat format = cursor.charFormat();
    if (format.objectType() == TagTextObject::TagTextFormat) {
        cursor.deleteChar(); // 删除TagTextObject
        emit tagDeleted(position);
        
        // 清空并重新扫描标签
        clearAllTags();
        textEdit->viewport()->update();
    } else {
        // 如果当前位置不是标签，尝试向前查找
        cursor.setPosition(position - 1);
        format = cursor.charFormat();
        if (format.objectType() ==  TagTextObject::TagTextFormat) {
            cursor.deleteChar();
            emit tagDeleted(position - 1);
            clearAllTags();
            textEdit->viewport()->update();
        }
    }
}

void RichTextEditorWidget::processDocumentForTags()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor(doc);
    
    // 遍历整个文档查找分号
    cursor.movePosition(QTextCursor::Start);
    
    while (!cursor.atEnd()) {        // 查找下一个分号（全角或半角）
        QTextCursor semicolonCursor = doc->find(QRegularExpression("[;；]"), cursor);
        
        if (semicolonCursor.isNull()) {
            break; // 没有找到更多分号
        }
        
        // 获取分号前的文本
        QString textBeforeSemicolon = getTextBeforeSemicolon(semicolonCursor);
        
        if (!textBeforeSemicolon.isEmpty()) {
            // 删除分号前的文本和分号本身
            QTextCursor deleteCursor = semicolonCursor;
            deleteCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, textBeforeSemicolon.length());
            deleteCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, textBeforeSemicolon.length() + 1); // +1 包含分号
            
            int deletePosition = deleteCursor.position() - deleteCursor.anchor();
            deleteCursor.removeSelectedText();
            
            // 在删除位置插入标签
            convertTextToPersonTag(textBeforeSemicolon);
            
            // 重新定位游标（因为文档已经被修改）
            cursor.setPosition(deleteCursor.position());
        } else {
            // 如果分号前没有有效文本，跳过这个分号
            cursor = semicolonCursor;
            cursor.movePosition(QTextCursor::Right);
        }
    }
}

QString RichTextEditorWidget::getTextBeforeSemicolon(const QTextCursor &semicolonCursor)
{
    QTextCursor cursor = semicolonCursor;
    QTextBlock currentBlock = cursor.block();
    
    // 获取分号在块中的位置
    int semicolonPosInBlock = cursor.positionInBlock();
    
    // 查找最后一个标签或行开始的位置
    int startPos = 0;
    
    // 遍历当前块，找到最后一个标签对象
    QTextBlock::iterator it;
    for (it = currentBlock.begin(); !it.atEnd(); ++it) {
        QTextFragment fragment = it.fragment();
        if (fragment.charFormat().objectType() == TagTextObject::TagTextFormat) {
            int tagEndPosInBlock = fragment.position() + fragment.length() - currentBlock.position();
            if (tagEndPosInBlock <= semicolonPosInBlock) {
                startPos = tagEndPosInBlock;
            }
        }
    }
    
    // 提取从最后一个标签到分号位置的文本
    QString blockText = currentBlock.text();
    QString textToConvert = blockText.mid(startPos, semicolonPosInBlock - startPos).trimmed();
    
    return textToConvert;
}

QString RichTextEditorWidget::getTextFromLastTagToCursor() const
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    int cursorPosition = cursor.position();
    
    // 从当前光标位置向前查找最近的标签
    int lastTagPosition = -1;
    
    // 遍历文档查找最近的标签位置
    QTextBlock block = doc->begin();
    while (block.isValid()) {
        QTextBlock::iterator it;
        for (it = block.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            int fragmentStart = fragment.position();
            int fragmentEnd = fragmentStart + fragment.length();
            
            // 如果片段在光标之前或包含光标，并且是标签
            if (fragmentStart < cursorPosition && 
                fragment.charFormat().objectType() == TagTextObject::TagTextFormat) {
                lastTagPosition = fragmentEnd; // 标签结束位置
            }
        }
        block = block.next();
    }
    
    // 如果没找到标签，从文档开始位置获取
    int startPosition = (lastTagPosition >= 0) ? lastTagPosition : 0;
    
    // 创建选择光标并获取文本
    QTextCursor selectCursor(doc);
    selectCursor.setPosition(startPosition);
    selectCursor.setPosition(cursorPosition, QTextCursor::KeepAnchor);
    
    return selectCursor.selectedText();
}

void RichTextEditorWidget::emitTextChangedFromLastTag()
{
    QString textFromLastTag = getTextFromLastTagToCursor();
    emit textChangedFromLastTag(textFromLastTag);
}

// 行高设置相关方法实现
void RichTextEditorWidget::setLineHeight(int height)
{
    // 确保行高在合理范围内
    if (height < MIN_LINE_HEIGHT) {
        height = MIN_LINE_HEIGHT;
    } else if (height > MAX_LINE_HEIGHT) {
        height = MAX_LINE_HEIGHT;
    }
    
    currentLineHeight = height;
    applyLineHeightToDocument();
}

int RichTextEditorWidget::getLineHeight() const
{
    return currentLineHeight;
}

void RichTextEditorWidget::resetLineHeight()
{
    currentLineHeight = DEFAULT_LINE_HEIGHT;
    applyLineHeightToDocument();
}

void RichTextEditorWidget::applyLineHeightToDocument()
{
    if (!textEdit || !textEdit->document()) {
        return;
    }
    
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor(doc);
    
    // 保存当前光标位置
    QTextCursor originalCursor = textEdit->textCursor();
    int originalPosition = originalCursor.position();
    
    // 选择整个文档
    cursor.select(QTextCursor::Document);
    
    // 创建块格式并设置行高
    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(currentLineHeight * LINE_SPACING_FACTOR, QTextBlockFormat::FixedHeight);
    
    // 创建字符格式并设置垂直对齐
    QTextCharFormat charFormat;
    charFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);
    
    // 应用格式到整个文档
    cursor.mergeBlockFormat(blockFormat);
    cursor.mergeCharFormat(charFormat);
    
    // 恢复原来的光标位置
    originalCursor.setPosition(originalPosition);
    textEdit->setTextCursor(originalCursor);
    
    // 重新调整高度
    adjustTextEditHeight();
}

void RichTextEditorWidget::triggerTagCreation(TagType tagType)
{
    QTextCursor cursor = textEdit->textCursor();
    QTextBlock currentBlock = cursor.block();
    int cursorPosInBlock = cursor.positionInBlock();
    
    // 查找光标前最后一个标签的位置
    int startPos = 0;
    
    // 遍历当前块，找到最后一个标签对象
    QTextBlock::iterator it;
    for (it = currentBlock.begin(); !it.atEnd(); ++it) {
        QTextFragment fragment = it.fragment();
        if (fragment.charFormat().objectType() == TagTextObject::TagTextFormat) {
            int tagEndPosInBlock = fragment.position() + fragment.length() - currentBlock.position();
            if (tagEndPosInBlock <= cursorPosInBlock) {
                startPos = tagEndPosInBlock;
            }
        }
    }
    
    // 提取从最后一个标签到光标位置的文本
    QString blockText = currentBlock.text();
    QString textToConvert = blockText.mid(startPos, cursorPosInBlock - startPos).trimmed();
    
    // 如果有文本需要转换
    if (!textToConvert.isEmpty()) {
        // 选择要转换的文本
        QTextCursor selectCursor = cursor;
        selectCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, textToConvert.length());
        selectCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, textToConvert.length());
        
        // 删除选中的文本
        selectCursor.removeSelectedText();
        
        // 在删除位置插入标签
        textEdit->setTextCursor(selectCursor);
        convertTextToPersonTag(textToConvert, tagType);
        
        // 触发文档更新处理
        emit textChanged();
        emitTextChangedFromLastTag();
    }
}

// 主题色设置相关方法实现
void RichTextEditorWidget::setThemeColor(const QColor &color)
{
    themeColor = color;
    // 如果当前文本框有焦点或鼠标悬停，立即更新边框颜色
    if (textEdit && (textEdit->hasFocus() || textEdit->underMouse())) {
        animateBorderColor(themeColor);
    }
}

QColor RichTextEditorWidget::getThemeColor() const
{
    return themeColor;
}

void RichTextEditorWidget::resetThemeColor()
{
    themeColor = QColor(70, 130, 180); // 默认钢蓝色
    // 如果当前文本框有焦点或鼠标悬停，立即更新边框颜色
    if (textEdit && (textEdit->hasFocus() || textEdit->underMouse())) {
        animateBorderColor(themeColor);
    }
}

void RichTextEditorWidget::updateStyleSheets()
{
    // 构建基础样式表模板
    QString baseStyleSheet = 
        "QTextEdit {"
        "   padding: 0px;"
        "   padding-left: 5px;"
        "   margin: 0px;"
        "   border: 1px solid %1;"
        "   border-radius: 4px;"
        "   background-color: white;"
        "}";
    
    // 设置当前边框颜色的样式
    textEdit->setStyleSheet(baseStyleSheet.arg(currentBorderColor.name()));
}

void RichTextEditorWidget::animateBorderColor(const QColor &targetColor)
{
    if (borderAnimation->state() == QAbstractAnimation::Running) {
        borderAnimation->stop();
    }
    
    borderAnimation->setStartValue(currentBorderColor);
    borderAnimation->setEndValue(targetColor);
    borderAnimation->start();
}

void RichTextEditorWidget::setBorderColor(const QColor &color)
{
    currentBorderColor = color;
    updateStyleSheets();
}

QColor RichTextEditorWidget::getBorderColor() const
{
    return currentBorderColor;
}

void RichTextEditorWidget::handleTagClick(const QPoint &mousePos)
{
    // 查找被点击的标签
    for (const auto &tag : allTags) {
        if (tag.rect.contains(mousePos) && !tag.deleteRect.contains(mousePos)) {
            // 发射标签点击信号
            emit tagClicked(tag.text, tag.position);
            break;
        }
    }
}
