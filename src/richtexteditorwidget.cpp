#include "richtexteditorwidget.h"
#include <QFontMetrics>
#include <QRegularExpression>
#include <QTextBlock>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

RichTextEditorWidget::RichTextEditorWidget(QWidget *parent)
    : QWidget(parent), minimumHeight(0), maximumHeight(300)
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
{    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // 创建富文本编辑器
    textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText("请输入文本内容...");
    textEdit->setAcceptRichText(true);
    textEdit->setMouseTracking(true);
    
    // 设置滚动条策略
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    
    // 计算初始最小高度
    QFontMetrics fm(textEdit->font());
    int lineHeight = fm.lineSpacing();
    minimumHeight = lineHeight + textEdit->contentsMargins().top() + textEdit->contentsMargins().bottom() + 10;
    textEdit->setMinimumHeight(minimumHeight);
    textEdit->setMaximumHeight(maximumHeight);
    
    // 注册自定义文本对象
    textEdit->document()->documentLayout()->registerHandler(TagTextObject::TagTextFormat, tagTextObject);
    
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
    });
    
    // 连接文档内容变化信号来自动调整高度
    connect(textEdit->document(), &QTextDocument::contentsChanged, 
            this, &RichTextEditorWidget::adjustTextEditHeight);
    
    // 初始化高度
    adjustTextEditHeight();
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
    int singleLineHeight = fm.height() + 10; // 添加一些边距
    
    // 设置最小高度为单行高度
    int newHeight = qMax(singleLineHeight, documentHeight);
    
    // 如果设置了最大高度，则限制高度
    if (maximumHeight > 0) {
        newHeight = qMin(newHeight, maximumHeight);
    }
    
    // 设置新的高度
    textEdit->setFixedHeight(newHeight);
    
    // 更新整个widget的高度
    updateGeometry();
}

// 事件过滤器
bool RichTextEditorWidget::eventFilter(QObject *obj, QEvent *event)
{
    // 处理键盘事件
    if (obj == textEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        // 检查是否按下了Ctrl+C复制快捷键
        if (keyEvent->matches(QKeySequence::Copy)) {
            handleCustomCopy();
            return true; // 阻止默认的复制行为
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

void RichTextEditorWidget::convertTextToPersonTag(const QString &text)
{
    QTextCursor cursor = textEdit->textCursor();
    
    // 创建自定义对象格式
    QTextCharFormat tagFormat;
    tagFormat.setObjectType(TagTextObject::TagTextFormat);
    tagFormat.setProperty(TagTextObject::TagProperty, text);
    
    // 在光标位置插入标签对象
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), tagFormat);
    
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

void RichTextEditorWidget::handleCustomCopy()
{
    QString selectedText = getSelectedTextWithTags();
    if (!selectedText.isEmpty()) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(selectedText);
    }
}

QString RichTextEditorWidget::getSelectedTextWithTags()
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection()) {
        return QString();
    }
    
    QString result;
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    
    // 遍历选中区域的所有字符
    QTextCursor tempCursor(textEdit->document());
    tempCursor.setPosition(start);
    
    while (tempCursor.position() < end) {
        QTextCharFormat format = tempCursor.charFormat();
        
        if (format.objectType() == TagTextObject::TagTextFormat) {
            // 这是一个标签对象，提取标签文本
            QString tagText = format.property(TagTextObject::TagProperty).toString();
            result += tagText;
            
            // 跳过这个对象字符
            tempCursor.movePosition(QTextCursor::NextCharacter);
        } else {
            // 这是普通文本
            result += tempCursor.document()->characterAt(tempCursor.position());
            tempCursor.movePosition(QTextCursor::NextCharacter);
        }
    }
    
    return result;
}
