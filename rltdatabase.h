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

#ifndef RLTDATABASE_H
#define RLTDATABASE_H

#include <QtSql/QtSql>
#include "singleton.h"
#include <QList>
#include <QHash>

class ModelRace;

class RLTDatabase: public Singleton<RLTDatabase>
{
    friend class Singleton<RLTDatabase>;
public:

    QSqlDatabase* database();
    static RLTDatabase* getInstance();
    void    createDummyPilot();
    int     createNewRace(QString);
    int     addPilotToRace(int,int);
    int     addLapTimeToRace(int,int,int,int);
    QList<ModelRace*> getRaces();
signals:

public slots:

private:
    RLTDatabase();

    bool            initDatabase();
    QSqlDatabase    m_sqliteDatabase;
};

#endif // RLTDATABASE_H
