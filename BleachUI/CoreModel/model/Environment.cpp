#include "Environment.h"
#include "Graph.h"
#include "Project.h"
#include <QRegularExpression>

model::Environment::Environment(Project* parent) : IdentifiableEntity(parent)
{
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

QString model::Environment::evaluate(QString workingUrl) const
{
	// TODO: implement an error handling mechanism
	// extract {vars}
	{
		QRegularExpression envVarPattern("{{([\\d\\w]+)}}");
		auto it = envVarPattern.globalMatch(workingUrl);
		while (it.hasNext())
		{
			auto match = it.next();
			auto name = match.captured(1);
		}
	}

	for (const auto& key : _entries.keys())
	{
		QRegularExpression pattern(QString("{{%1}}").arg(key));
		workingUrl = workingUrl.replace(pattern, _entries[key].toString());
	}

	return workingUrl;
}

QJSValue model::Environment::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	auto value = PersistableEntity::saveToJSValue(persistenceHandler);
	auto entriesValue = persistenceHandler->createJsValue();	
	for(auto key : getEntries().keys())
	{
		entriesValue.setProperty(key, persistenceHandler->createJsValue(getEntries().value(key)));
	}
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
