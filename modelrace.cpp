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

#include "modelrace.h"
#include "rltdatabase.h"
#include <QSqlDatabase>
#include <QSqlQuery>

ModelRace::ModelRace()
{

}

int ModelRace::getId(){
    return m_Id;
}

QString ModelRace::getName(){
    return m_Name;
}

QDateTime ModelRace::getCreatedAt(){
    return m_Created_at;
}

void ModelRace::setId(int v){
    m_Id = v;
}

void ModelRace::setName(QString v){
    m_Name = v;
}

void ModelRace::setCreatedAt(QDateTime v){
    m_Created_at = v;
}


int ModelRace::getNumPilots(){
    QSqlDatabase *db = RLTDatabase::instance()->database();

    QSqlQuery numQuery(*db);
    if(!numQuery.exec(QString("SELECT count(*) FROM race_pilots WHERE race_id=%1;").arg(m_Id))){
        qDebug() << "ModelRace::getNumPilots(): failed - " << numQuery.lastError();
        return 0;
    }

    if(numQuery.next()){
        return numQuery.value(0).toInt();
    }
    return 0;
}


QString ModelRace::getFormatedFastedLapTime(){
    QSqlDatabase *db = RLTDatabase::instance()->database();

    QSqlQuery numQuery(*db);
    if(!numQuery.exec(QString("SELECT * FROM race_laps WHERE race_id=%1 ORDER BY lap_time ASC;").arg(m_Id))){
        qDebug() << "ModelRace::getFormatedFastedLapTime(): failed - " << numQuery.lastError();
        return 0;
    }

    if(numQuery.next()){
        int ms =  numQuery.value(3).toInt();
        return QString("%1").arg(ms / 1000.0f);
    }
    return "";
}

QString ModelRace::getFastestPilotName(){
    QSqlDatabase *db = RLTDatabase::instance()->database();

    QSqlQuery numQuery(*db);
    if(!numQuery.exec(QString("SELECT * FROM race_laps WHERE race_id=%1 ORDER BY lap_time ASC;").arg(m_Id))){
        qDebug() << "ModelRace::getFormatedFastedLapTime(): failed - " << numQuery.lastError();
        return 0;
    }

    if(numQuery.next()){
        int race_pilot_id =  numQuery.value(2).toInt();


        QSqlQuery race_pilots_query(*db);
        if(race_pilots_query.exec(QString("SELECT * FROM race_pilots WHERE id=%1;").arg(race_pilot_id))){

            if(race_pilots_query.next()){
                int pilot_id =  race_pilots_query.value(1).toInt();
                QSqlQuery pilot_query(*db);

                if(pilot_query.exec(QString("SELECT * FROM pilots WHERE id=%1").arg(pilot_id))){
                    if(pilot_query.next()){
                        return pilot_query.value(1).toString();
                    }
                }else{
                    qDebug() << "ModelRace::getFastestPilotName(): failed pilot_query- " << pilot_query.lastError();
                }
            }
        }else{
            qDebug() << "ModelRace::getFastestPilotName(): failed race_pilots_query- " << race_pilots_query.lastError();
        }
    }
    return "";
}
