#pragma once
#include <QJSEngine>
#include <QJSValue>

#include <QObject>

namespace model
{
    class CustomJSEngine : public QJSEngine
    {
        Q_OBJECT
    public:
        CustomJSEngine();
    private:
        bool injectJSPathLib();
        bool injectAssertionLib();
    };
}
