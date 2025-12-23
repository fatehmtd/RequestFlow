#include "JSONHighlighter.h"
#include "SyntaxColors.h"

view::JSONHighlighter::JSONHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // JSON keys (property names) - do this FIRST with lookahead
    keyFormat.setForeground(SyntaxColors::jsonKey());
    keyFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\"[^\"]*\"(?=\\s*:)"));
    rule.format = keyFormat;
    highlightingRules.append(rule);

    // String values - do this AFTER keys (won't override keys due to lookahead)
    stringFormat.setForeground(SyntaxColors::jsonString());
    rule.pattern = QRegularExpression(QStringLiteral("(?<=:)\\s*\"(?:[^\"\\\\]|\\\\.)*\""));
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Numbers
    numberFormat.setForeground(SyntaxColors::jsonNumber());
    rule.pattern = QRegularExpression(QStringLiteral(":\\s*-?\\d+\\.?\\d*([eE][+-]?\\d+)?"));
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Boolean and null values (combined)
    booleanFormat.setForeground(SyntaxColors::jsonBoolean());
    booleanFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\b(true|false|null)\\b"));
    rule.format = booleanFormat;
    highlightingRules.append(rule);

    // Symbols (combined into single pattern)
    symbolFormat.setForeground(SyntaxColors::jsonSymbol());
    rule.pattern = QRegularExpression(QStringLiteral("[{}\\[\\]:,]"));
    rule.format = symbolFormat;
    highlightingRules.append(rule);
}

void view::JSONHighlighter::highlightBlock(const QString& text)
{
    // Skip if highlighting is disabled
    if (!m_enabled) {
        return;
    }

    // Skip highlighting for very long lines (performance optimization)
    if (text.length() > 10000) {
        return;
    }

    // Apply all rules
    for (const HighlightingRule& rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
