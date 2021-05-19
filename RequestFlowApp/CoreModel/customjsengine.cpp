#include "customjsengine.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

model::CustomJSEngine::CustomJSEngine()
{
    installExtensions(QJSEngine::Extension::AllExtensions);
    injectJSPathLib();
    injectAssertionLib();
}

bool model::CustomJSEngine::injectJSPathLib()
{
    QFile fp(":/js/jsonpath");
    if (fp.open(QIODevice::ReadOnly))
    {
        QTextStream data(&fp);
        evaluate(data.readAll());
        //qDebug() << __FUNCTION__ << v.toString();
        globalObject().setProperty("pathOf", evaluate("(function (path, obj) { return JSONPath.JSONPath(path, obj);})"));
        return true;
    }
    return false;
}

bool model::CustomJSEngine::injectAssertionLib()
{
    QFile fp(":/js/assert");
    if (fp.open(QIODevice::ReadOnly))
    {
        QTextStream data(&fp);
        qDebug() << evaluate(QString("%1").arg(data.readAll())).toString();
        return true;
    }
    return false;
}
