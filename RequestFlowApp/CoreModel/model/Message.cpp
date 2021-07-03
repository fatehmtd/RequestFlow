#include "Message.h"

#include <QDebug>
#include <QJSEngine>
#include <QJSValue>
#include <QJsonDocument>
#include <QJsonObject>

model::Message::Message(QString body) : _body(body) {}

model::Message::Message(const Message &m)
    : _pathVars(m._pathVars), _queryParams(m._queryParams),
      _context(m._context), _body(m._body) {}

model::Message::~Message() {}

void model::Message::setHeaders(const QMap<QString, QVariant> &headers) {
  _headers = headers;
}

QMap<QString, QVariant> model::Message::getHeaders() const { return _headers; }

QMap<QString, QVariant> &model::Message::getHeaders() { return _headers; }

void model::Message::setPathVars(const QMap<QString, QVariant> &pathVars) {
  _pathVars = pathVars;
}

QMap<QString, QVariant> model::Message::getPathVars() const {
  return _pathVars;
}

QMap<QString, QVariant> &model::Message::getPathVars() { return _pathVars; }

void model::Message::setContext(const QMap<QString, QVariant> &context) {
  _context = context;
}

QMap<QString, QVariant> model::Message::getContext() const { return _context; }

QMap<QString, QVariant> &model::Message::getContext() { return _context; }

void model::Message::setQueryParams(
    const QMap<QString, QVariant> &queryParams) {
  _queryParams = queryParams;
}

QMap<QString, QVariant> model::Message::getQueryParams() const {
  return _queryParams;
}

QMap<QString, QVariant> &model::Message::getQueryParams() {
  return _queryParams;
}

QString model::Message::getBody() const { return _body; }

void model::Message::setBody(const QString &body) { this->_body = body; }

QVariant model::Message::toVariant() const {
  QMap<QString, QVariant> output;
  output["body"] = JSONParse(getBody());
  output["path"] = getPathVars();
  output["query"] = getQueryParams();
  output["context"] = getContext();
  output["headers"] = getHeaders();
  return QVariant(output);
}

void model::Message::fromVariant(const QVariant &v) {
  auto map = v.value<QMap<QString, QVariant>>();
  setBody(JSONStringify(map["body"]));
  setQueryParams(map["query"].value<QMap<QString, QVariant>>());
  setPathVars(map["path"].value<QMap<QString, QVariant>>());
  setContext(map["context"].value<QMap<QString, QVariant>>());
  setHeaders(map["headers"].value<QMap<QString, QVariant>>());
}

model::Message &model::Message::operator=(const Message &m) {
  _body = m.getBody();
  _queryParams = m.getQueryParams();
  _pathVars = m.getPathVars();
  _context = m.getContext();
  _headers = m.getHeaders();
  return *this;
}

QString model::Message::JSONStringify(const QVariant &v) {
  switch (v.type()) {
  case QVariant::Type::Map:
  case QVariant::Type::List:
  case QVariant::Type::StringList:
  case QVariant::Type::Hash: {
    QJSEngine engine;
    auto scriptValue = engine.toScriptValue(v);
    engine.globalObject().setProperty("value", scriptValue);
    auto result = engine.evaluate("value");
    auto resultValue = engine.fromScriptValue<QVariant>(result);
    return QString(QJsonDocument::fromVariant(resultValue)
                       .toJson(QJsonDocument::JsonFormat::Compact));
  } break;
  default:
    return v.toString();
    break;
  }
}

QVariant model::Message::JSONParse(const QString &s) {
  QJSEngine engine;
  auto result = engine.evaluate(QString("(%1)").arg(s));
  if (!result.isError()) {
    return engine.fromScriptValue<QVariant>(result);
  }
  return s;
}
