#include "HTTPHighlighter.h"
#include "SyntaxColors.h"

view::HTTPHighlighter::HTTPHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // HTTP methods (GET, POST, etc.) - at start of line
    httpMethodFormat.setForeground(QColor(175, 0, 219));  // Magenta
    httpMethodFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("^(GET|POST|PUT|DELETE|PATCH|HEAD|OPTIONS|CONNECT|TRACE)\\b"));
    rule.format = httpMethodFormat;
    highlightingRules.append(rule);

    // URLs (http:// or https://)
    urlFormat.setForeground(QColor(0, 119, 190));  // Blue
    urlFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("https?://[^\\s]+"));
    rule.format = urlFormat;
    highlightingRules.append(rule);

    // Status codes (Status : 200)
    statusCodeFormat.setForeground(QColor(0, 128, 0));  // Green
    statusCodeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=Status\\s*:\\s*)\\d{3}"));
    rule.format = statusCodeFormat;
    highlightingRules.append(rule);

    // Header names (everything before colon at start of line)
    headerNameFormat.setForeground(QColor(0, 92, 197));  // Blue
    headerNameFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("^[^:\\s][^:]*(?=\\s*:)"));
    rule.format = headerNameFormat;
    highlightingRules.append(rule);

    // Numbers in values (status codes, content lengths, etc.)
    numberFormat.setForeground(QColor(175, 0, 219));  // Magenta
    rule.pattern = QRegularExpression(QStringLiteral("(?<=:\\s*)\\d+(?:\\s*ms)?\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void view::HTTPHighlighter::highlightBlock(const QString& text)
{
    // Apply all rules
    for (const HighlightingRule& rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
