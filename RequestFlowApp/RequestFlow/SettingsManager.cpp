#include "SettingsManager.h"

view::SettingsManager::SettingsManager(QObject* p) : QObject(p), _settings(new QSettings("FBMZ", "RequestFlow", this))
{

}

view::SettingsManager::~SettingsManager()
{

}

void view::SettingsManager::clearRecentProjects()
{
    _settings->remove("recentProjects");
}

QList<QString> view::SettingsManager::enumRecentProjects() const
{
    auto recentProjectsList = _settings->value("recentProjects").toList();
    QList<QString> output;
    for (const auto& v : recentProjectsList)
    {
        output.append(v.toString());
    }
    return output;
}

void view::SettingsManager::removeRecentProject(const QString& path)
{
    auto recentProjectsList = _settings->value("recentProjects").toList();
    if (recentProjectsList.contains(path))
    {
        recentProjectsList.removeAll(path);
        _settings->setValue("recentProjects", recentProjectsList);
    }
}

void view::SettingsManager::addRecentProject(const QString& path)
{
    auto recentProjectsList = _settings->value("recentProjects").toList();
    if (recentProjectsList.contains(path))
    {
        recentProjectsList.removeAll(path);
    }
    recentProjectsList.prepend(path);
    _settings->setValue("recentProjects", recentProjectsList);
}

QString view::SettingsManager::getLastOpenedLocation() const
{
    auto recentProjectsList = _settings->value("recentProjects").toList();
    return recentProjectsList.isEmpty() ? "" : recentProjectsList.first().toString();
}

bool view::SettingsManager::addEditorEntry(QString name, QString path)
{
    emit editorEntryAdded(name);
    return false;
}

void view::SettingsManager::removeEditorEntry(QString name)
{
    emit editorEntryRemoved(name);
}

view::SettingsManager::EditorEntry view::SettingsManager::getEditorEntry(QString name) const
{
    return EditorEntry();
}

QList<view::SettingsManager::EditorEntry> view::SettingsManager::getEditorEntries() const
{
    return QList<EditorEntry>();
}

void view::SettingsManager::setBackgroundType(uint type)
{
    setEntry("backgroundType", type);
}

uint view::SettingsManager::getBackgroundType() const
{
    return getEntry("backgroundType", 3).toUInt();
}

void view::SettingsManager::setEdgesStyle(uint style)
{
    setEntry("edgeStyle", style);
}

uint view::SettingsManager::getEdgesStyle() const
{
    return getEntry("edgeStyle", 0).toUInt();
}

void view::SettingsManager::setMiniMapLocation(int location)
{
    setEntry("MiniMapLocation", location);
}

int view::SettingsManager::getMiniMapLocation() const
{
    return getEntry("MiniMapLocation", 0).toInt();
}

void view::SettingsManager::setMiniMapStatus(bool status)
{
    setEntry("MiniMapStatus", status);
}

bool view::SettingsManager::getMiniMapStatus() const
{
    return getEntry("MiniMapStatus", true).toBool();
}

void view::SettingsManager::setProxyStatus(bool status)
{
    setEntry("proxy.status", status);
}

bool view::SettingsManager::getProxyStatus() const
{
    return getEntry("proxy.status", false).toBool();
}

void view::SettingsManager::setProxyConfig(ProxyConfig config)
{
    setEntry("proxy.hostName", config.hostName);
    setEntry("proxy.port", config.port);
    setEntry("proxy.authNeeded", config.authNeeded);
    setEntry("proxy.userName", config.userName);
    setEntry("proxy.password", config.password);
    setEntry("proxy.proxyType", config.proxyType);
}

view::SettingsManager::ProxyConfig view::SettingsManager::getProxyConfig() const
{
    ProxyConfig config;
    config.hostName = getEntry("proxy.hostName", "").toString();
    config.port = getEntry("proxy.port", 8080).toUInt();
    config.authNeeded = getEntry("proxy.authNeeded", false).toBool();
    config.userName = getEntry("proxy.userName", "").toString();
    config.password = getEntry("proxy.password", "").toString();
    config.proxyType = getEntry("proxy.proxyType", 3).toInt();
    return config;
}

#include <QNetworkProxy>

void view::SettingsManager::applyProxySetting() const
{
    auto config = getProxyConfig();
    QNetworkProxy proxy;
    if(getProxyStatus())
    {
        proxy.setType((QNetworkProxy::ProxyType)config.proxyType);
        proxy.setPort(config.port);
        proxy.setHostName(config.hostName);
        if(config.authNeeded)
        {
            proxy.setUser(config.userName);
            proxy.setPassword(config.password);
        }
    }
    else
    {
        proxy.setType(QNetworkProxy::ProxyType::NoProxy);
    }

    QNetworkProxy::setApplicationProxy(proxy);
}

void view::SettingsManager::setEntry(const QString &name, const QVariant &value)
{
    _settings->setValue(name, value);
    emit entrySet(name);
}

QVariant view::SettingsManager::getEntry(const QString &name, const QVariant &defaultValue) const
{
    if(!_settings->contains(name)) return defaultValue;
    return _settings->value(name);
}
