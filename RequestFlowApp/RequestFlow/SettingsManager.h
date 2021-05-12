#pragma once

#include <qlist.h>
#include <QSettings>
#include <memory>

namespace view
{
	class SettingsManager : public QObject
	{
	public:
		SettingsManager(QObject* parent);
		~SettingsManager();

		void clearRecentProjects();
		QList<QString> enumRecentProjects() const;
		//////////////////////////////////////////////////////////////////////////
		void removeRecentProject(const QString& path);
		void addRecentProject(const QString& path);
		QString getLastOpenedLocation() const;
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::unique_ptr<QSettings> _settings;
	};
}
