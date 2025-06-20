// 使用RichTextEditorWidget控件的示例

#include "richtexteditorwidget.h"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ExampleMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ExampleMainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        // 创建中央控件
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        
        // 添加标题
        QLabel *title = new QLabel("富文本编辑器控件使用示例", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
        layout->addWidget(title);
        
        // 创建富文本编辑器控件
        richTextEditor = new RichTextEditorWidget(this);
        layout->addWidget(richTextEditor);
        
        // 添加一些操作按钮
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        
        QPushButton *getTextBtn = new QPushButton("获取纯文本", this);
        QPushButton *getHtmlBtn = new QPushButton("获取HTML", this);
        QPushButton *clearBtn = new QPushButton("清空", this);
        QPushButton *setTextBtn = new QPushButton("设置示例文本", this);
        
        buttonLayout->addWidget(getTextBtn);
        buttonLayout->addWidget(getHtmlBtn);
        buttonLayout->addWidget(clearBtn);
        buttonLayout->addWidget(setTextBtn);
        buttonLayout->addStretch();
        
        layout->addLayout(buttonLayout);
        
        // 连接按钮事件
        connect(getTextBtn, &QPushButton::clicked, [this]() {
            QString text = richTextEditor->getPlainText();
            qDebug() << "纯文本内容:" << text;
        });
        
        connect(getHtmlBtn, &QPushButton::clicked, [this]() {
            QString html = richTextEditor->getHtmlText();
            qDebug() << "HTML内容:" << html;
        });
        
        connect(clearBtn, &QPushButton::clicked, [this]() {
            richTextEditor->clear();
        });
        
        connect(setTextBtn, &QPushButton::clicked, [this]() {
            richTextEditor->setPlainText("这是一个示例文本，你可以在这里编辑并添加人员标签。");
        });
        
        // 监听富文本编辑器的信号
        connect(richTextEditor, &RichTextEditorWidget::textChanged, [this]() {
            qDebug() << "文本内容已变化";
        });
        
        connect(richTextEditor, &RichTextEditorWidget::tagInserted, [this](const QString &tagText) {
            qDebug() << "插入了新标签:" << tagText;
        });
        
        setWindowTitle("富文本编辑器控件示例");
        resize(800, 600);
    }

private:
    RichTextEditorWidget *richTextEditor;
};

// 如果要单独运行这个示例，可以使用以下main函数
/*
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    ExampleMainWindow window;
    window.show();
    
    return app.exec();
}
*/

#include "example_usage.moc"
