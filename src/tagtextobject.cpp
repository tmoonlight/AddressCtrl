#include "tagtextobject.h"
#include "richtexteditorwidget.h"
#include <QFontMetrics>
#include <QApplication>
#include <QPainterPath>

// ????????????????

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
    QFontMetrics fm(font);
      // ??????????????
    int width = fm.horizontalAdvance(text) + 20 + 6; // ???10????? + ???3????
    int height = fm.height() + 8; // ???4?????
    
    return QSizeF(width, height);
}

void TagTextObject::drawObject(QPainter *painter, const QRectF &rect,
                              QTextDocument *doc, int posInDocument,
                              const QTextFormat &format)
{
    Q_UNUSED(doc)
      QString text = format.property(TagProperty).toString();
    
    // ???????????
    if (tagManager) {
        tagManager->updateTagRect(posInDocument, rect, text);
    }
    
    painter->save();
      // ?????
    painter->setRenderHint(QPainter::Antialiasing);
    
    // ???????????3?????
    QRectF drawRect = rect.adjusted(3, 0, -3, 0);
    
    // ????????
    QPainterPath path;
    path.addRoundedRect(drawRect, 12, 12);    // ??????
    QLinearGradient gradient(drawRect.topLeft(), drawRect.bottomLeft());    // ??????????????????????
    bool isHovered = false;
    if (currentMousePos.x() >= 0 && currentMousePos.y() >= 0 && tagManager) {
        int hoveredPos = tagManager->getHoveredTagPosition(currentMousePos);
        isHovered = (hoveredPos == posInDocument);
    }
    
    if (isHovered) {
        // ??????? (RGB * 0.5)
        gradient.setColorAt(0, QColor(35, 65, 90, 200));   // ??? * 0.5
        gradient.setColorAt(1, QColor(50, 74, 118, 200));  // ???? * 0.5
    } else {
        // ????
        gradient.setColorAt(0, QColor(70, 130, 180, 200));  // ???
        gradient.setColorAt(1, QColor(100, 149, 237, 200)); // ????
    }
    
    painter->fillPath(path, QBrush(gradient));
    
    // ????
    painter->setPen(QPen(QColor(70, 130, 180), 1));
    painter->drawPath(path);
    
    // ????
    painter->setPen(QColor(255, 255, 255)); // ????
    QFont font = QApplication::font();
    font.setPointSize(10);
    font.setBold(true);
    painter->setFont(font);
    
    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter->drawText(drawRect, text, option);
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