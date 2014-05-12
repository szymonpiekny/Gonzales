#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QSettings>
#include <QVariant>
#include <QStringList>
#include "../common.h"

class IApplicationSettings
{
public:
    virtual void setValue(const QString &key, const QVariant &value)=0;
    virtual bool contains(const QString &key) const =0;
    virtual void remove(const QString &key)=0;
    virtual QVariant value(const QString &key, const QVariant &value) const=0;
    virtual void beginGroup(const QString &str) = 0;
    virtual QString group() = 0;
    virtual QStringList childKeys() const = 0;
    virtual QStringList allKeys() const = 0;
    virtual QStringList childGroups() const = 0;
};

class ApplicationSettings: public IApplicationSettings
{
public:
    ApplicationSettings():
        settings(g_Company, g_Project)
    {
    }

    QString group()
    {
        return settings.group();
    }

    void setValue(const QString &key, const QVariant &value)
    {
        settings.setValue(key, value);
    }

    bool contains(const QString &key)const
    {
        return settings.contains(key);
    }

    void remove(const QString &key)
    {
        settings.remove(key);
    }

    QVariant value(const QString &key, const QVariant &value) const
    {
        return settings.value(key, value);
    }

    void beginGroup(const QString &str)
    {
        settings.beginGroup(str);
    }

    QStringList childKeys() const
    {
        return settings.childKeys();
    }

    QStringList allKeys() const
    {
        return settings.allKeys();
    }

    QStringList childGroups() const
    {
        return settings.childGroups();
    }

private:
    QSettings settings;
};

#endif // APPLICATIONSETTINGS_H
