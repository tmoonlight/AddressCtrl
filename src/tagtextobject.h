#ifndef TAGTEXTOBJECT_H
#define TAGTEXTOBJECT_H

#include <QObject>
#include <QTextObjectInterface>
#include <QPainter>
#include <QTextFormat>
#include <QTextDocument>
#include <QRectF>
#include <QVector>

// ??????
struct TagInfo {
    QRectF rect;        // ????
    int position;       // ???????
    QString text;       // ????
    bool isHovered;     // ??????
    
    TagInfo() : position(-1), isHovered(false) {}
    TagInfo(const QRectF &r, int pos, const QString &t) 
        : rect(r), position(pos), text(t), isHovered(false) {}
};

class TagTextObject : public QObject, public QTextObjectInterface
{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    enum { TagTextFormat = QTextFormat::UserObject + 1 };
    enum { TagProperty = 1 }; // ??HoverProperty?????

    explicit TagTextObject(class RichTextEditorWidget* manager = nullptr);

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                        const QTextFormat &format) override;

    void drawObject(QPainter *painter, const QRectF &rect,
                   QTextDocument *doc, int posInDocument,
                   const QTextFormat &format) override;
                   
    // ???????
    void setTagManager(class RichTextEditorWidget* manager);
    
    // ??????
    void setCurrentMousePos(const QPoint &pos);
    QPoint getCurrentMousePos() const;
    
private:
    QPoint currentMousePos;                    // ??????
    class RichTextEditorWidget* tagManager;   // ?????
};

#endif // TAGTEXTOBJECT_H
