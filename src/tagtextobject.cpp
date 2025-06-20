#include "tagtextobject.h"
#include <QFontMetrics>
#include <QApplication>
#include <QPainterPath>

QSizeF TagTextObject::intrinsicSize(QTextDocument *doc, int posInDocument,
                                   const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    
    QString text = format.property(TagProperty).toString();
    QFont font = QApplication::font();
    font.setPointSize(10);
    QFontMetrics fm(font);
    
    // 计算文本尺寸，添加一些内边距
    int width = fm.horizontalAdvance(text) + 20; // 左右各10像素内边距
    int height = fm.height() + 8; // 上下各4像素内边距
    
    return QSizeF(width, height);
}

void TagTextObject::drawObject(QPainter *painter, const QRectF &rect,
                              QTextDocument *doc, int posInDocument,
                              const QTextFormat &format)
{
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    
    QString text = format.property(TagProperty).toString();
    
    painter->save();
    
    // 设置抗锯齿
    painter->setRenderHint(QPainter::Antialiasing);
    
    // 绘制圆角矩形背景
    QPainterPath path;
    path.addRoundedRect(rect, 12, 12);
    
    // 设置渐变背景
    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
    gradient.setColorAt(0, QColor(70, 130, 180, 200));  // 钢蓝色
    gradient.setColorAt(1, QColor(100, 149, 237, 200)); // 矢车菊蓝
    
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
    painter->drawText(rect, text, option);
    
    painter->restore();
}

