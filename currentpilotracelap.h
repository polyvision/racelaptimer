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

#ifndef CURRENTPILOTRACELAP_H
#define CURRENTPILOTRACELAP_H

#include <QDateTime>

class CurrentPilotRaceLap
{
public:
    CurrentPilotRaceLap(int);

    void        setLapStart(QDateTime);
    void        setLapEnd(QDateTime);
    QDateTime   getLapStart();
    QDateTime   getLapEnd();

    qint64      lapTime();
    QString     formatedLapTime();

    bool        finishLap();
    void        startLap();
    bool        isFinished();
    int         getLap();
private:
    int         m_iLap;
    QDateTime   m_timeLapStart;
    QDateTime   m_timeLapEnd;
    bool        m_bFinishedLap;
    bool        m_bFastedPilotLap;
};

#endif // CURRENTPILOTRACELAP_H
