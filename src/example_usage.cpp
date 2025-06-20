// ??RichTextEditorWidget?????

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
        // ??????
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        
        // ????
        QLabel *title = new QLabel("????????????", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
        layout->addWidget(title);
        
        // ??????????
        richTextEditor = new RichTextEditorWidget(this);
        layout->addWidget(richTextEditor);
        
        // ????????
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        
        QPushButton *getTextBtn = new QPushButton("?????", this);
        QPushButton *getHtmlBtn = new QPushButton("??HTML", this);
        QPushButton *clearBtn = new QPushButton("??", this);
        QPushButton *setTextBtn = new QPushButton("??????", this);
        
        buttonLayout->addWidget(getTextBtn);
        buttonLayout->addWidget(getHtmlBtn);
        buttonLayout->addWidget(clearBtn);
        buttonLayout->addWidget(setTextBtn);
        buttonLayout->addStretch();
        
        layout->addLayout(buttonLayout);
        
        // ??????
        connect(getTextBtn, &QPushButton::clicked, [this]() {
            QString text = richTextEditor->getPlainText();
            qDebug() << "?????:" << text;
        });
        
        connect(getHtmlBtn, &QPushButton::clicked, [this]() {
            QString html = richTextEditor->getHtmlText();
            qDebug() << "HTML??:" << html;
        });
        
        connect(clearBtn, &QPushButton::clicked, [this]() {
            richTextEditor->clear();
        });
        
        connect(setTextBtn, &QPushButton::clicked, [this]() {
            richTextEditor->setPlainText("?????????????????????????");
        });
        
        // ???????????
        connect(richTextEditor, &RichTextEditorWidget::textChanged, [this]() {
            qDebug() << "???????";
        });
        
        connect(richTextEditor, &RichTextEditorWidget::tagInserted, [this](const QString &tagText) {
            qDebug() << "??????:" << tagText;
        });
        
        setWindowTitle("??????????");
        resize(800, 600);
    }

private:
    RichTextEditorWidget *richTextEditor;
};

// ??????????????????main??
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
