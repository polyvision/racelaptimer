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

#include "currentpilotracelap.h"
#include <QDebug>

CurrentPilotRaceLap::CurrentPilotRaceLap(int lap)
{
    m_iLap = lap;
    m_bFinishedLap = false;
}

void CurrentPilotRaceLap::setLapStart(QDateTime v){
    this->m_timeLapStart = v;
}

void CurrentPilotRaceLap::setLapEnd(QDateTime v){
    this->m_timeLapEnd = v;
}

QDateTime CurrentPilotRaceLap::getLapStart(){
    return this->m_timeLapStart;
}

QDateTime CurrentPilotRaceLap::getLapEnd(){
    return this->m_timeLapEnd;
}

void CurrentPilotRaceLap::startLap(){
    this->m_timeLapStart = QDateTime::currentDateTime();
}

bool CurrentPilotRaceLap::finishLap(){
    QDateTime now = QDateTime::currentDateTime();

    // there must be a difference of one second to finish a lap
    if(this->m_timeLapStart.msecsTo(now) > 1000){
        m_bFinishedLap = true;
        this->m_timeLapEnd = now;
        return true;
    }
    return false;
}

qint64 CurrentPilotRaceLap::lapTime(){
    //qDebug() << this->m_timeLapStart.toString();
    //qDebug() << this->m_timeLapEnd.toString();
    return this->m_timeLapStart.msecsTo(this->m_timeLapEnd);
}

QString CurrentPilotRaceLap::formatedLapTime(){
    //qDebug() << this->lapTime();
    return QString("%1").arg(this->lapTime() / 1000.0f);
}

bool CurrentPilotRaceLap::isFinished(){
    return m_bFinishedLap;
}

int CurrentPilotRaceLap::getLap(){
    return m_iLap;
}
