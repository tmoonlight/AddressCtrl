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
#include <QColor>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include "tagtextobject.h"

// 行高设置宏定义
#define DEFAULT_LINE_HEIGHT 24        // 默认行高（像素）
#define LINE_SPACING_FACTOR 1.2       // 行间距因子
#define MIN_LINE_HEIGHT 12             // 最小行高（像素）
#define MAX_LINE_HEIGHT 36             // 最大行高（像素）

// 标签类型枚举
enum class TagType {
    Normal,    // 普通标签（白底）
    Error      // 错误标签（红色风格）
};

class RichTextEditorWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)

public:
    explicit RichTextEditorWidget(QWidget *parent = nullptr);
    ~RichTextEditorWidget();
      // 公共接口
    QString getPlainText() const;
    QString getHtmlText() const;
    QString getCompleteText(bool isTagOnly = false) const; // 获取包含标签内容的完整文本，isTagOnly为true时只获取标签内容
    void setPlainText(const QString &text);
    void setHtmlText(const QString &html);
    void clear();
    
    // 获取内部textEdit指针（如果需要直接访问）
    QTextEdit* getTextEdit() const;
    
    // 高度调整设置
    void setMaximumTextHeight(int maxHeight);
    int getMaximumTextHeight() const;
    
    // 行高设置接口
    void setLineHeight(int height);
    int getLineHeight() const;
    void resetLineHeight();
      // 主题色设置接口
    void setThemeColor(const QColor &color);
    QColor getThemeColor() const;
    void resetThemeColor();
    
    // 边框颜色属性（用于动画）
    void setBorderColor(const QColor &color);
    QColor getBorderColor() const;

    // 标签操作接口
    void triggerTagCreation(TagType tagType = TagType::Normal);  // 触发将光标前的文本转换为标签（类似输入分号的效果）
      // 标签管理接口（供TagTextObject回调使用）
    void updateTagRect(int position, const QRectF &rect, const QString &text);
    void updateTagDeleteButtonRect(int position, const QRectF &deleteRect);
    int getHoveredTagPosition(const QPoint &mousePos) const;
    bool isPointOnDeleteButton(const QPoint &mousePos) const;
    void deleteTagAtPosition(int position);
    void clearAllTags();

signals:
    void textChanged();
    void tagInserted(const QString &tagText);
    void tagDeleted(int position);
    void tagClicked(const QString &tagText, int position); // 新增：标签被点击的信号
    void textChangedFromLastTag(const QString &textFromLastTag); // 新增：从上一个tag到当前光标的文本内容

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupUI();
    void convertTextToPersonTag(const QString &text, TagType tagType = TagType::Normal);
    void processDocumentForTags(); // 新增：扫描文档并处理分号转换标签
    QString getTextBeforeSemicolon(const QTextCursor &semicolonCursor); // 新增：获取分号前的文本
    QString getTextFromLastTagToCursor() const; // 新增：获取从上一个tag到当前光标的文本
    void emitTextChangedFromLastTag(); // 新增：发射textChangedFromLastTag信号
    void adjustTextEditHeight();
    void applyLineHeightToDocument(); // 新增：应用行高设置到文档
    
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
    
    // 行高设置
    int currentLineHeight;  // 当前行高设置
      // 主题色设置
    QColor themeColor;      // 主题色
    QColor currentBorderColor;  // 当前边框颜色
    QString normalStyleSheet;   // 正常状态的样式表
    QString focusStyleSheet;    // 焦点状态的样式表
    QPropertyAnimation *borderAnimation; // 边框颜色动画
    
    // 私有方法
    void updateStyleSheets();   // 更新样式表
    void animateBorderColor(const QColor &targetColor); // 动画边框颜色
    void updateBorderColor(); // 更新边框颜色（动画过程中调用）
    void handleTagClick(const QPoint &mousePos); // 处理标签点击
};

#endif // RICHTEXTEDITORWIDGET_H
