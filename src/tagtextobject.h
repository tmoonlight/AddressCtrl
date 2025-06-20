#ifndef TAGTEXTOBJECT_H
#define TAGTEXTOBJECT_H

#include <QObject>
#include <QTextObjectInterface>
#include <QPainter>
#include <QTextFormat>
#include <QTextDocument>
#include <QRectF>
#include <QVector>

// 标签信息结构
struct TagInfo {
    QRectF rect;        // 绘制矩形
    int position;       // 在文档中的位置
    QString text;       // 标签文本
    bool isHovered;     // 是否悬停状态
    
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
    enum { TagProperty = 1 }; // 移除HoverProperty，不再需要

    explicit TagTextObject(class RichTextEditorWidget* manager = nullptr);

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                        const QTextFormat &format) override;

    void drawObject(QPainter *painter, const QRectF &rect,
                   QTextDocument *doc, int posInDocument,
                   const QTextFormat &format) override;
                   
    // 设置管理器接口
    void setTagManager(class RichTextEditorWidget* manager);
    
    // 鼠标位置相关
    void setCurrentMousePos(const QPoint &pos);
    QPoint getCurrentMousePos() const;
    
private:
    QPoint currentMousePos;                    // 当前鼠标位置
    class RichTextEditorWidget* tagManager;   // 标签管理器
};

#endif // TAGTEXTOBJECT_H
