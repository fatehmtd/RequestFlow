#include "JSCompleter.h"
#include <QAbstractItemView>
#include <QDebug>

view::JSCompleter::JSCompleter(QPlainTextEdit* parent)
    : QCompleter(parent), _textEdit(parent)
{
    _model = new QStringListModel(this);
    setModel(_model);
    setWidget(_textEdit);
    setCompletionMode(QCompleter::PopupCompletion);
    setCaseSensitivity(Qt::CaseInsensitive);
    setWrapAround(false);
    
    setupCompletions();
    
    _textEdit->installEventFilter(this);
    connect(_textEdit, &QPlainTextEdit::textChanged, this, &JSCompleter::onTextChanged);
    connect(this, QOverload<const QString&>::of(&QCompleter::activated),
            this, &JSCompleter::insertCompletion);
}

void view::JSCompleter::setupCompletions()
{
    QStringList completions;
    
    // JavaScript keywords
    completions << "break" << "case" << "catch" << "class" << "const" << "continue"
                << "debugger" << "default" << "delete" << "do" << "else" << "export"
                << "extends" << "finally" << "for" << "function" << "if" << "import"
                << "in" << "instanceof" << "let" << "new" << "return" << "super"
                << "switch" << "this" << "throw" << "try" << "typeof" << "var"
                << "void" << "while" << "with" << "yield"
                << "await" << "async" << "of";
    
    // Built-in objects - RequestFlow specific
    completions << "Request" << "Response" 
                << "Request.body" << "Request.query" << "Request.path" << "Request.context"
                << "Response.body" << "Response.query" << "Response.path" << "Response.context";
    
    // JavaScript built-in objects
    completions << "Array" << "Boolean" << "Date" << "Error" << "Function" << "JSON"
                << "Math" << "Number" << "Object" << "Promise" << "RegExp" << "String"
                << "Symbol" << "Map" << "Set" << "WeakMap" << "WeakSet";
    
    // Common methods
    completions << "console.log" << "console.error" << "console.warn" << "console.info"
                << "JSON.parse" << "JSON.stringify"
                << "Math.abs" << "Math.ceil" << "Math.floor" << "Math.max" << "Math.min"
                << "Math.random" << "Math.round"
                << "parseInt" << "parseFloat" << "isNaN" << "isFinite"
                << "toString" << "valueOf" << "length" << "push" << "pop" << "shift"
                << "unshift" << "slice" << "splice" << "filter" << "map" << "reduce"
                << "forEach" << "find" << "findIndex" << "includes" << "indexOf"
                << "join" << "reverse" << "sort" << "concat";
    
    completions.sort();
    _model->setStringList(completions);
}

QString view::JSCompleter::textUnderCursor() const
{
    QTextCursor tc = _textEdit->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void view::JSCompleter::insertCompletion(const QString& completion)
{
    if (widget() != _textEdit)
        return;
    
    QTextCursor tc = _textEdit->textCursor();
    
    // Get the text under cursor to determine how much to replace
    QString textUnder = textUnderCursor();
    int extra = completion.length() - textUnder.length();
    
    tc.select(QTextCursor::WordUnderCursor);
    tc.insertText(completion);
    _textEdit->setTextCursor(tc);
}

bool view::JSCompleter::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        
        // If completion popup is visible
        if (popup()->isVisible()) {
            // Keys that should be handled by the completer
            switch (keyEvent->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                event->ignore();
                return true;
            default:
                break;
            }
        }
        
        // Ctrl+Space triggers completion
        if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_Space) {
            QString prefix = textUnderCursor();
            if (prefix != completionPrefix()) {
                setCompletionPrefix(prefix);
                popup()->setCurrentIndex(completionModel()->index(0, 0));
            }
            
            QRect cr = _textEdit->cursorRect();
            cr.translate(_textEdit->viewport()->mapToGlobal(QPoint(0, 0)));
            cr.setWidth(popup()->sizeHintForColumn(0)
                        + popup()->verticalScrollBar()->sizeHint().width());
            complete(cr);
            return true;
        }
    }
    
    return QCompleter::eventFilter(obj, event);
}

void view::JSCompleter::onTextChanged()
{
    QString currentPrefix = textUnderCursor();
    
    // Only show completion if we have at least 2 characters
    if (currentPrefix.length() < 2) {
        popup()->hide();
        return;
    }
    
    if (currentPrefix != completionPrefix()) {
        setCompletionPrefix(currentPrefix);
        popup()->setCurrentIndex(completionModel()->index(0, 0));
    }
    
    // Show popup if we have matches
    if (completionCount() > 0) {
        QRect cr = _textEdit->cursorRect();
        cr.translate(_textEdit->viewport()->mapToGlobal(QPoint(0, 0)));
        cr.setWidth(popup()->sizeHintForColumn(0)
                    + popup()->verticalScrollBar()->sizeHint().width());
        complete(cr);
    } else {
        popup()->hide();
    }
}
