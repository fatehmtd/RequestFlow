#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

namespace view
{
    class JSHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT

    public:
        JSHighlighter(QTextDocument* parent = nullptr);
        void setEnabled(bool enabled) { m_enabled = enabled; }

    protected:
        void highlightBlock(const QString& text) override;

    private:
        struct HighlightingRule
        {
            QRegularExpression pattern;
            QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;

        QRegularExpression commentStartExpression;
        QRegularExpression commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat builtInObjectFormat;
        QTextCharFormat builtInPropertyFormat;
        QTextCharFormat objectKeyFormat;
        QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat functionFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat operatorFormat;
        bool m_enabled = true;
    };
}
