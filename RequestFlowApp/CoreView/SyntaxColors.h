#pragma once
#include <QColor>

namespace view
{
    namespace SyntaxColors
    {
        // JavaScript/Code colors - vibrant for light backgrounds
        inline QColor keyword()   { return QColor(0, 119, 190); }    // Deep blue - Keywords
        inline QColor string()    { return QColor(163, 21, 21); }    // Deep red - Strings
        inline QColor number()    { return QColor(175, 0, 219); }     // Teal/green - Numbers
        inline QColor comment()   { return QColor(0, 128, 0); }      // Pure green - Comments
        inline QColor builtInObject() { return QColor(156, 220, 254); }  // Cyan - Built-in objects (Request, Response)
        inline QColor builtInProperty() { return QColor(78, 201, 176); } // Turquoise - Built-in properties (.body, .query, etc.)
        
        // JSON specific colors - vibrant for light backgrounds
        inline QColor jsonKey()      { return QColor(0, 92, 197); }    // Bright blue - Object Properties
        inline QColor jsonString()   { return QColor(163, 21, 21); }   // Deep red - String values
        inline QColor jsonNumber()   { return QColor(175, 0, 219); }    // Teal/green - Numbers
        inline QColor jsonBoolean()  { return QColor(175, 0, 219); }   // Magenta/purple - Booleans/null
        inline QColor jsonSymbol()   { return QColor(100, 100, 100); } // Dark gray - Symbols
    }
}
