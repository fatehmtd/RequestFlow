#pragma once
#include <QJSEngine>
#include <QJSValue>

#include <QObject>
#include "coremodel_global.h"

namespace model
{
    class COREMODEL_EXPORT CustomJSEngine : public QJSEngine
    {
        Q_OBJECT
    public:
        CustomJSEngine();
    private:
        bool injectJSPathLib();
        bool injectAssertionLib();
    };
}
