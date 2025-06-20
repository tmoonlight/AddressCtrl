#ifndef TAGTEXTOBJECT_H
#define TAGTEXTOBJECT_H

#include <QObject>
#include <QTextObjectInterface>
#include <QPainter>
#include <QTextFormat>
#include <QTextDocument>

class TagTextObject : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    enum { TagTextFormat = QTextFormat::UserObject + 1 };
    enum { TagProperty = 1 };

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                        const QTextFormat &format) override;
    void drawObject(QPainter *painter, const QRectF &rect,
                   QTextDocument *doc, int posInDocument,
                   const QTextFormat &format) override;
};

#endif // TAGTEXTOBJECT_H
