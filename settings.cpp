#include "settings.h"

Settings::Settings()
{

}

void Settings::setLapBeepPath(QString v){
    m_Settings.setValue("lap_beep_path",v);
}

void Settings::setFastestLapSoundPath(QString v){
    m_Settings.setValue("fastest_lap_sound_path",v);
}

QString Settings::getLapBeepPath(){
    return m_Settings.value("lap_beep_path").toString();
}

QString Settings::getFastestLapSoundPath(){
    return m_Settings.value("fastest_lap_sound_path").toString();
}
