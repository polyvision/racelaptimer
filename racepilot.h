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

#ifndef RACEPILOT_H
#define RACEPILOT_H

#include <QObject>
#include <QList>
#include <QDateTime>

class CurrentPilotRaceLap;

class RacePilot : public QObject
{
    Q_OBJECT
public:
    explicit RacePilot(QObject *parent = 0);

    void        setQuadToken(QString);
    void        setPilotName(QString);
    void        setDatabaseID(QString);
    QString     getPilotName();
    QString     getQuadToken();
    int         getID();
    int         lapCount();
    QDateTime   currentLapStartTime();
    QString     lastLapTimeString();
    bool        hasLastLapTime();
    CurrentPilotRaceLap* getFastedLap();
    void        fireLapTime();
    void        setCurrentRaceID(int);

    static RacePilot* getByQuadToken(QString);
signals:

public slots:

private:
    void    startLap();
    void    finishLap();

    QString m_strQuadToken;
    QString m_strPilotName;
    QString m_strDatabaseID;
    int     m_iCurrentRaceID;

    QList<CurrentPilotRaceLap*> m_listLaps;
    CurrentPilotRaceLap*    m_pCurrentRaceLap;
};

#endif // RACEPILOT_H
