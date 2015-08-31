#ifndef SETTINGS_H
#define SETTINGS_H

#include "singleton.h"
#include <QSettings>

class Settings: public Singleton<Settings>
{
    friend class Singleton<Settings>;

public:
    Settings();
    void    setLapBeepPath(QString v);
    void    setFastestLapSoundPath(QString v);

    QString getLapBeepPath();
    QString getFastestLapSoundPath();

signals:

public slots:
private:
    QSettings m_Settings;
};

#endif // SETTINGS_H
