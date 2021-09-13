#pragma once

#include <qlist.h>
#include <QSettings>
#include <memory>

namespace view
{
	class SettingsManager : public QObject
	{
        Q_OBJECT
	public:
		SettingsManager(QObject* parent);
        ~SettingsManager();

        //////////////////////////////////////////////////////////////////////////
        /// Recent projects
        ///
		void removeRecentProject(const QString& path);
		void addRecentProject(const QString& path);
        QString getLastOpenedLocation() const;
        void clearRecentProjects();
        QList<QString> enumRecentProjects() const;

        ////////////////////////////////////////////////////////////////////////
        /// External Editors
        ///
        struct EditorEntry
        {
            QString name;
            QString path;
        };

        bool addEditorEntry(QString name, QString path);
        void removeEditorEntry(QString name);
        EditorEntry getEditorEntry(QString name) const;
        QList<EditorEntry> getEditorEntries() const;

        ///////////////////////////////////////////////////////////////////////
        /// Background
        ///
        void setBackgroundType(uint type);
        uint getBackgroundType() const;

        void setEdgesStyle(uint style);
        uint getEdgesStyle() const;

        ///////////////////////////////////////////////////////////////////////
        /// Theme
        ///
        enum Theme {
            LIGHT,
            DARK
        };

        void setTheme(Theme theme);
        Theme getTheme() const;
        ///////////////////////////////////////////////////////////////////////
        /// MiniMap
        ///
        void setMiniMapLocation(int location);
        int getMiniMapLocation() const;

        void setMiniMapStatus(bool status);
        bool getMiniMapStatus() const;

        //////////////////////////////////////////////////////////////////////
        /// Proxy
        ///
        struct ProxyConfig
        {
            QString hostName;
            QString userName;
            QString password;
            bool authNeeded;
            quint16 port;
            int proxyType; // http or socks
        };

        void setProxyStatus(bool status);
        bool getProxyStatus() const;

        void setProxyConfig(ProxyConfig config);
        ProxyConfig getProxyConfig() const;

        void applyProxySetting() const;

        //////////////////////////////////////////////////////////////////////
        /// Generic set/get entries
        ///
        void setEntry(const QString& name, const QVariant& value);
        QVariant getEntry(const QString& name, const QVariant& defaultValue = QVariant()) const;

    signals:
        void editorEntryAdded(const QString&);
        void editorEntryRemoved(const QString&);

        void entrySet(const QString&);

    protected:
        QSettings *_settings=nullptr;
	};
}
