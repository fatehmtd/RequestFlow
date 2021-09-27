#include "Environment.h"
#include "Graph.h"
#include "Project.h"
#include <QRegularExpression>

model::Environment::Environment(Project* parent) : IdentifiableEntity(parent)
{
    getEntries()["baseUrl"] = "http://localhost";
    getEntries()["basic_auth_user"] = "admin";
    getEntries()["basic_auth_pwd"] = "pwd";
    getEntries()["bearer_token"] = "bearer-token";
}

model::Environment::Environment(const Environment& original) : IdentifiableEntity(original.getProject()), _entries(original.getEntries())
{
}

model::Project* model::Environment::getProject() const
{
	return dynamic_cast<Project*>(parent());
}

void model::Environment::setEntries(const QMap<QString, QVariant>& entries)
{
	_entries = entries;
}

QMap<QString, QVariant>& model::Environment::getEntries()
{
	return _entries;
}

QMap<QString, QVariant> model::Environment::getEntries() const
{
	return _entries;
}

QString model::Environment::evaluate(QString workingUrl) const {
    if(_entries.isEmpty()) return workingUrl;

    const auto& entriesKeys = _entries.keys();

    std::for_each(entriesKeys.begin(), entriesKeys.end(), [&workingUrl, this](const QString& key){
        QRegularExpression pattern(QString("{{%1}}").arg(key));
        workingUrl = workingUrl.replace(pattern, _entries[key].toString());
    });

	return workingUrl;
}

QJSValue model::Environment::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	auto value = PersistableEntity::saveToJSValue(persistenceHandler);
    auto entriesValue = persistenceHandler->createJsValue();

    std::for_each(getEntries().keyBegin(), getEntries().keyEnd(), [=, &entriesValue](const QString& key)
                  {
                      entriesValue.setProperty(key, persistenceHandler->createJsValue(getEntries().value(key)));
                  });
	value.setProperty("entries", entriesValue);
	return value;
}

#include <QJSValueIterator>

bool model::Environment::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);
	auto entriesValue = v.property("entries");
	QJSValueIterator it(entriesValue);
	while (it.hasNext())
	{
		it.next();
		auto entryName = QString(it.name());
		auto entryValue = entriesValue.property(entryName).toVariant();
		getEntries().insert(entryName, entryValue);
	}
	return true;
}
