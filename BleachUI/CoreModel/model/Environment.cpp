#include "Environment.h"
#include "Graph.h"
#include "Project.h"
#include <QRegularExpression>

model::Environment::Environment(Project* parent) : QObject(parent)
{
}

model::Environment::Environment(const Environment& original) : QObject(original.getProject()), _entries(original.getEntries())
{
}

model::Project* model::Environment::getProject() const
{
	return dynamic_cast<Project*>(parent());
}

void model::Environment::setName(const QString& name)
{
	_name = name;
}

QString model::Environment::getName() const
{
	return _name;
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
