#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

namespace view
{
    class JSONHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT

    public:
        JSONHighlighter(QTextDocument* parent = nullptr);
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

        QTextCharFormat keyFormat;
        QTextCharFormat stringFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat booleanFormat;
        QTextCharFormat nullFormat;
        QTextCharFormat symbolFormat;
        bool m_enabled = true;
    };
}
