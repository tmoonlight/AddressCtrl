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
    QFont font = QApplication::font();
    font.setPointSize(10);
    QFontMetrics fm(font);    // 计算文本尺寸，添加一些内边距和删除按钮的空间
    int width = fm.horizontalAdvance(text) + 20 + 6 + 16; // 左右各10像素内边距 + 左右各3像素留白 + 16像素删除按钮
    int height = fm.height() + 8; // 上下各4像素内边距
    
    return QSizeF(width, height);
}

void TagTextObject::drawObject(QPainter *painter, const QRectF &rect,
                              QTextDocument *doc, int posInDocument,
                              const QTextFormat &format)
{
    Q_UNUSED(doc)
      QString text = format.property(TagProperty).toString();
    
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
    
    // 绘制圆角矩形背景
    QPainterPath path;
    path.addRoundedRect(drawRect, 3, 3);    // 设置渐变背景
    QLinearGradient gradient(drawRect.topLeft(), drawRect.bottomLeft());    // 使用更精确的悬停检测：检查当前标签是否被悬停
    bool isHovered = false;
    if (currentMousePos.x() >= 0 && currentMousePos.y() >= 0 && tagManager) {
        int hoveredPos = tagManager->getHoveredTagPosition(currentMousePos);
        isHovered = (hoveredPos == posInDocument);
    }
    
    if (isHovered) {
        // 悬停时背景变浅 (RGB * 0.5)
        gradient.setColorAt(0, QColor(35, 65, 90, 200));   // 钢蓝色 * 0.5
        gradient.setColorAt(1, QColor(50, 74, 118, 200));  // 矢车菊蓝 * 0.5
    } else {
        // 正常状态
        gradient.setColorAt(0, QColor(70, 130, 180, 200));  // 钢蓝色
        gradient.setColorAt(1, QColor(100, 149, 237, 200)); // 矢车菊蓝
    }
    
    painter->fillPath(path, QBrush(gradient));
    
    // 绘制边框
    painter->setPen(QPen(QColor(70, 130, 180), 1));
    painter->drawPath(path);
      // 绘制文本
    painter->setPen(QColor(255, 255, 255)); // 白色文字
    QFont font = QApplication::font();
    font.setPointSize(10);
    font.setBold(true);
    painter->setFont(font);
    
    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter->drawText(textRect, text, option);
    
    // 绘制删除按钮
    painter->setPen(QPen(QColor(255, 255, 255), 1.5));
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