#include "tagtextobject.h"
#include "richtexteditorwidget.h"
#include <QFontMetrics>
#include <QApplication>
#include <QPainterPath>

// 静态变量定义已移除，改为实例变量

TagTextObject::TagTextObject(RichTextEditorWidget* manager)
    : currentMousePos(-1, -1), tagManager(manager)
{
}

QSizeF TagTextObject::intrinsicSize(QTextDocument *doc, int posInDocument,
                                   const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    
    QString text = format.property(TagProperty).toString();
    
    // 获取标签类型
    int tagTypeInt = format.property(TagTypeProperty).toInt();
    bool isErrorTag = (tagTypeInt == 1); // 1 代表 TagType::Error
    
    QFont font = QApplication::font();
    font.setPointSize(10);
    QFontMetrics fm(font);
    
    // 计算文本尺寸，添加一些内边距和删除按钮的空间
    int baseWidth = fm.horizontalAdvance(text) + 20 + 6 + 16; // 左右各10像素内边距 + 左右各3像素留白 + 16像素删除按钮
    
    // 如果是错误标签，为感叹号图标预留额外空间
    if (isErrorTag) {
        baseWidth += 18; // 12像素图标 + 6像素间距
    }
    
    int height = fm.height() + 8; // 上下各4像素内边距
    
    return QSizeF(baseWidth, height);
}

void TagTextObject::drawObject(QPainter *painter, const QRectF &rect,
                              QTextDocument *doc, int posInDocument,
                              const QTextFormat &format)
{
    Q_UNUSED(doc)
    
    QString text = format.property(TagProperty).toString();
    
    // 获取标签类型
    int tagTypeInt = format.property(TagTypeProperty).toInt();
    bool isErrorTag = (tagTypeInt == 1); // 1 代表 TagType::Error
    
    // 通过管理器更新标签矩形
    if (tagManager) {
        tagManager->updateTagRect(posInDocument, rect, text);
    }
    
    painter->save();
    
    // 设置抗锯齿
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 调整绘制区域，留出左右3像素的空白
    QRectF drawRect = rect.adjusted(3, 0, -3, 0);
    
    // 计算删除按钮区域（右侧）
    int deleteButtonSize = 14;
    QRectF deleteButtonRect = QRectF(
        drawRect.right() - deleteButtonSize - 4, 
        drawRect.top() + (drawRect.height() - deleteButtonSize) / 2,
        deleteButtonSize, 
        deleteButtonSize
    );
    
    // 调整文本绘制区域，为删除按钮留出空间
    QRectF textRect = drawRect.adjusted(0, 0, -deleteButtonSize - 6, 0);
    
    // 使用更精确的悬停检测：检查当前标签是否被悬停
    bool isHovered = false;
    if (currentMousePos.x() >= 0 && currentMousePos.y() >= 0 && tagManager) {
        int hoveredPos = tagManager->getHoveredTagPosition(currentMousePos);
        isHovered = (hoveredPos == posInDocument);
    }
    
    // 绘制圆角矩形背景
    QPainterPath path;
    path.addRoundedRect(drawRect, 3, 3);
    
    // 设置背景色和边框色
    QColor backgroundColor;
    QColor borderColor;
    QColor textColor;
    
    if (isErrorTag) {
        // 错误标签的红色风格
        if (isHovered) {
            backgroundColor = QColor(252, 226, 226); // 更深的红色hover背景
            borderColor = QColor(245, 108, 108); // 更深的红色边框
        } else {
            backgroundColor = QColor(254, 240, 240); // 浅红色背景
            borderColor = QColor(252, 165, 165); // 红色边框
        }
        textColor = QColor(153, 27, 27); // 深红色文字
    } else {
        // 普通标签的白色风格
        if (isHovered) {
            backgroundColor = QColor(245, 245, 245); // hover背景色
        } else {
            backgroundColor = QColor(255, 255, 255); // 白色背景
        }
        borderColor = QColor("#e9e9e9"); // 浅灰色边框
        textColor = QColor("#606266"); // 深灰色文字
    }
    
    painter->fillPath(path, QBrush(backgroundColor));
    
    // 绘制边框 - 使用0.5像素宽度和像素对齐以获得清晰效果
    painter->setPen(QPen(borderColor, 0.5));
    // 调整矩形到像素网格对齐
    QRectF alignedRect = QRectF(
        qRound(drawRect.x()) + 0.5, 
        qRound(drawRect.y()) + 0.5,
        qRound(drawRect.width()) - 1, 
        qRound(drawRect.height()) - 1
    );
    QPainterPath alignedPath;
    alignedPath.addRoundedRect(alignedRect, 3, 3);
    painter->drawPath(alignedPath);
    
    // 绘制文本和图标
    painter->setPen(textColor);
    QFont font = QApplication::font();
    font.setPointSize(10);
    font.setBold(false); // 不使用粗体
    painter->setFont(font);
    
    if (isErrorTag) {
        // 错误标签：绘制感叹号图标 + 文本
        // 计算感叹号圆圈的大小和位置
        int iconSize = 12;
        QRectF iconRect = QRectF(
            textRect.left() + 4,
            textRect.top() + (textRect.height() - iconSize) / 2,
            iconSize,
            iconSize
        );
        
        // 绘制圆圈背景
        painter->setBrush(QBrush(textColor));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(iconRect);
        
        // 绘制感叹号
        painter->setPen(QPen(backgroundColor, 1.5));
        QPointF center = iconRect.center();
        // 感叹号的主体部分
        painter->drawLine(center.x(), center.y() - 3, center.x(), center.y() + 1);
        // 感叹号的点
        painter->drawPoint(center.x(), center.y() + 2.5);
        
        // 调整文本绘制区域，为图标留出空间
        QRectF adjustedTextRect = textRect.adjusted(iconSize + 6, 0, 0, 0);
        
        // 绘制文本
        painter->setPen(textColor);
        QTextOption option;
        option.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        painter->drawText(adjustedTextRect, text, option);
    } else {
        // 普通标签：居中绘制文本
        QTextOption option;
        option.setAlignment(Qt::AlignCenter);
        painter->drawText(textRect, text, option);
    }
    
    // 绘制删除按钮
    painter->setPen(QPen(textColor, 1.5)); // 删除按钮使用与文本相同的颜色
    int crossSize = 6;
    QPointF center = deleteButtonRect.center();
    // 绘制 X 形状的删除图标
    painter->drawLine(
        center.x() - crossSize/2, center.y() - crossSize/2,
        center.x() + crossSize/2, center.y() + crossSize/2
    );
    painter->drawLine(
        center.x() - crossSize/2, center.y() + crossSize/2,
        center.x() + crossSize/2, center.y() - crossSize/2
    );
    
    // 通过管理器更新删除按钮区域
    if (tagManager) {
        tagManager->updateTagDeleteButtonRect(posInDocument, deleteButtonRect);
    }
    
    painter->restore();
}

void TagTextObject::setCurrentMousePos(const QPoint &pos)
{
    currentMousePos = pos;
}

void TagTextObject::setTagManager(RichTextEditorWidget* manager)
{
    tagManager = manager;
}

QPoint TagTextObject::getCurrentMousePos() const
{
    return currentMousePos;
}