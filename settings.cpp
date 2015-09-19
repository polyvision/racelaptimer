/**
 * OpenRaceLapTimer - Copyright 2015 by airbirds.de, a project of polyvision UG (haftungsbeschränkt)
 *
 * Author: Alexander B. Bierbrauer
 *
 * This file is part of OpenRaceLapTimer.
 *
 * OpenRaceLapTimer is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * OpenRaceLapTimer is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Foobar. If not, see http://www.gnu.org/licenses/.
 **/

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

void Settings::setTrackingTimeout(int v){
    m_Settings.setValue("tracking_timeout_ms",v);
}

int Settings::getTrackingTimeout(){
    int v = m_Settings.value("tracking_timeout_ms").toInt();

    if (v < 100){
        return 100;
    }

    return v;
}
