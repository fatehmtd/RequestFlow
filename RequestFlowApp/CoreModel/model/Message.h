#pragma once

#include "../coremodel_global.h"

#include <QMap>
#include <QVariant>

namespace model {
class COREMODEL_EXPORT Message {
public:
  explicit Message(QString body = "");
  Message(const Message &m);
  ~Message();

  void setPathVars(const QMap<QString, QVariant> &pathVars);
  QMap<QString, QVariant> getPathVars() const;
  QMap<QString, QVariant> &getPathVars();

  void setHeaders(const QMap<QString, QVariant> &headers);
  QMap<QString, QVariant> getHeaders() const;
  QMap<QString, QVariant> &getHeaders();

  void setContext(const QMap<QString, QVariant> &context);
  QMap<QString, QVariant> getContext() const;
  QMap<QString, QVariant> &getContext();

  void setQueryParams(const QMap<QString, QVariant> &queryParams);
  QMap<QString, QVariant> getQueryParams() const;
  QMap<QString, QVariant> &getQueryParams();

  QString getBody() const;
  void setBody(const QString &body);

  QVariant toVariant() const;
  void fromVariant(const QVariant &v);

  Message &operator=(const Message &m);

  void printMe() const;

  static QString JSONStringify(const QVariant &v);
  static QVariant JSONParse(const QString &s);

private:
  QMap<QString, QVariant> _pathVars, _queryParams, _context, _headers;
  QString _body;
};
} // namespace model
