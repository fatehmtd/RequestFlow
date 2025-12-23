#pragma once
#include <QCompleter>
#include <QPlainTextEdit>
#include <QStringListModel>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QTextCursor>

namespace view
{
    class JSCompleter : public QCompleter
    {
        Q_OBJECT

    public:
        explicit JSCompleter(QPlainTextEdit* parent = nullptr);
        
        void setupCompletions();
        bool eventFilter(QObject* obj, QEvent* event) override;
        
    private:
        QPlainTextEdit* _textEdit;
        QStringListModel* _model;
        
        QString textUnderCursor() const;
        void insertCompletion(const QString& completion);
        
    private slots:
        void onTextChanged();
    };
}
