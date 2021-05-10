#include "SettingsManager.h"

view::SettingsManager::SettingsManager(QObject* p) : QObject(p), _settings(std::make_unique<QSettings>("FBMZ", "RequestFlow"))
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
	for (auto v : recentProjectsList)
	{
		output.append(v.toString());
	}
	return std::move(output);
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
