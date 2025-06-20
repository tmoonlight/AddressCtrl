#ifndef RICHTEXTEDITORWIDGET_H
#define RICHTEXTEDITORWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QKeyEvent>
#include <QTextBlock>
#include <QMouseEvent>
#include "tagtextobject.h"

class RichTextEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RichTextEditorWidget(QWidget *parent = nullptr);
    ~RichTextEditorWidget();
    
    // 公共接口
    QString getPlainText() const;
    QString getHtmlText() const;
    void setPlainText(const QString &text);
    void setHtmlText(const QString &html);
    void clear();
    
    // 获取内部textEdit指针（如果需要直接访问）
    QTextEdit* getTextEdit() const;
    
    // 高度调整设置
    void setMaximumTextHeight(int maxHeight);
    int getMaximumTextHeight() const;
    
    // 标签管理接口（供TagTextObject回调使用）
    void updateTagRect(int position, const QRectF &rect, const QString &text);
    int getHoveredTagPosition(const QPoint &mousePos) const;
    void clearAllTags();

signals:
    void textChanged();
    void tagInserted(const QString &tagText);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupUI();
    void convertTextToPersonTag(const QString &text);
    void adjustTextEditHeight();
    
    // UI组件
    QVBoxLayout *mainLayout;
    QTextEdit *textEdit;
    
    // 标签文本对象
    TagTextObject *tagTextObject;
    
    // 标签管理
    QVector<TagInfo> allTags;  // 当前编辑器的所有标签信息
    
    // 自动高度调整
    int minimumHeight;
    int maximumHeight;
};

#endif // RICHTEXTEDITORWIDGET_H
