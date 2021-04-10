#pragma once

#include "../coremodel_global.h"
#include <QObject>
#include <QMap>
#include <QVariant>

namespace model
{
	class Project;

	class COREMODEL_EXPORT Environment : public QObject
	{
	public:
		Environment(Project* parent);
		Environment(const Environment& original);

		Project* getProject() const;

		void setName(const QString& name);
		QString getName() const;

		void setEntries(const QMap<QString, QVariant>& entries);
		QMap<QString, QVariant>& getEntries();
		QMap<QString, QVariant> getEntries() const;

		QString evaluate(QString input) const; // replaces {key} by the corresponding entry

	private:
		QString _name;
		QMap<QString, QVariant> _entries;
	};
}