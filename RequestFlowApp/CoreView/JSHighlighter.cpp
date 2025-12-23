#include "JSHighlighter.h"
#include "SyntaxColors.h"

view::JSHighlighter::JSHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Multi-line comments - must be set up first
    multiLineCommentFormat.setForeground(SyntaxColors::comment());
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

    // Built-in object properties (Request.body, Response.query, etc.) - BEFORE objects
    builtInPropertyFormat.setForeground(SyntaxColors::builtInProperty());
    builtInPropertyFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(
        QStringLiteral("\\b(Request|Response)\\.(body|query|path|context)\\b"));
    rule.format = builtInPropertyFormat;
    highlightingRules.append(rule);

    // Built-in objects (Request, Response) - AFTER properties to avoid conflict
    builtInObjectFormat.setForeground(SyntaxColors::builtInObject());
    builtInObjectFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\b(Request|Response)\\b"));
    rule.format = builtInObjectFormat;
    highlightingRules.append(rule);

    // Object keys in JavaScript (quoted keys before colon)
    objectKeyFormat.setForeground(SyntaxColors::jsonKey());
    objectKeyFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\"[^\"]*\"(?=\\s*:)"));
    rule.format = objectKeyFormat;
    highlightingRules.append(rule);

    // Keywords - combined into single pattern for better performance
    keywordFormat.setForeground(SyntaxColors::keyword());
    keywordFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(
        QStringLiteral("\\b(var|let|const|function|return|if|else|for|while|do|switch|case|"
                       "break|continue|try|catch|finally|throw|new|this|delete|typeof|"
                       "instanceof|in|of|await|async|class|extends|static|super|import|"
                       "export|default|from|as|yield|void|null|undefined|true|false)\\b"));
    rule.format = keywordFormat;
    highlightingRules.append(rule);

    // Numbers
    numberFormat.setForeground(SyntaxColors::number());
    rule.pattern = QRegularExpression(QStringLiteral("\\b[0-9]+\\.?[0-9]*\\b"));
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Strings (combined pattern for better performance)
    quotationFormat.setForeground(SyntaxColors::string());
    rule.pattern = QRegularExpression(
        QStringLiteral("\"(?:[^\"\\\\]|\\\\.)*\"|'(?:[^'\\\\]|\\\\.)*'|`(?:[^`\\\\]|\\\\.)*`"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Single line comments - MUST BE LAST in rules so they override everything
    singleLineCommentFormat.setForeground(SyntaxColors::comment());
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
}

void view::JSHighlighter::highlightBlock(const QString& text)
{
    // Skip if highlighting is disabled
    if (!m_enabled) {
        return;
    }

    // Skip highlighting for very long lines (performance optimization)
    if (text.length() > 10000) {
        return;
    }

    // Apply all single-line rules FIRST
    for (const HighlightingRule& rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // Handle multi-line comments LAST (so they override everything inside comments)
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
