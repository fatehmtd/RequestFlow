#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

namespace view
{
    class HTTPHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT

    public:
        HTTPHighlighter(QTextDocument* parent = nullptr);

    protected:
        void highlightBlock(const QString& text) override;

    private:
        struct HighlightingRule
        {
            QRegularExpression pattern;
            QTextCharFormat format;
        };
        QVector<HighlightingRule> highlightingRules;

        QTextCharFormat httpMethodFormat;
        QTextCharFormat urlFormat;
        QTextCharFormat statusCodeFormat;
        QTextCharFormat headerNameFormat;
        QTextCharFormat headerValueFormat;
        QTextCharFormat numberFormat;
    };
}
