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


        void setBackgroundType(uint type);
        uint getBackgroundType() const;

        void setEdgesStyle(uint style);
        uint getEdgesStyle() const;

        void setEntry(const QString& name, const QVariant& value);
        QVariant getEntry(const QString& name, const QVariant& defaultValue = QVariant()) const;
		//////////////////////////////////////////////////////////////////////////
	protected:
        QSettings *_settings=nullptr;
	};
}
