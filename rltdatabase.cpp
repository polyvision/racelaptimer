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

#include "rltdatabase.h"
#include <QDebug>
#include "modelrace.h"

RLTDatabase::RLTDatabase()
{
    m_sqliteDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqliteDatabase.setDatabaseName(QDir::homePath() + QDir::separator() + "race_lap_timer.db3");

    if(m_sqliteDatabase.open()){
        // successfully opened the database, let's init the db
        this->initDatabase();
    }else{
        qDebug() << "RLTDatabase::contstructor(): failed to open database";
    }
}

QSqlDatabase* RLTDatabase::database(){
    return &m_sqliteDatabase;
}

bool RLTDatabase::initDatabase(){
    QSqlQuery query(m_sqliteDatabase);
    QString strQueryPilotsTable = "CREATE TABLE pilots(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT NOT NULL,api_token CHAR(50),quad_token CHAR(50));";
    QString strQueryRacesTable = "CREATE TABLE races(id INTEGER PRIMARY KEY AUTOINCREMENT,title TEXT NOT NULL,created_at TEXT NOT NULL);";
    QString strQueryRacePilotsTable = "CREATE TABLE race_pilots(id INTEGER PRIMARY KEY AUTOINCREMENT,pilot_id INTEGER,race_id INTEGER);";
    QString strQueryRaceLapsTable = "CREATE TABLE race_laps(id INTEGER PRIMARY KEY AUTOINCREMENT,race_id INTEGER,race_pilot_id INTEGER,lap_time INTEGER,lap_count INTEGER);";

    if(query.exec(strQueryPilotsTable)){
        qDebug() << "RLTDatabase::initDatabase(): created pilots table";
    }else{
        qDebug() << "RLTDatabase::initDatabase(): failed to create pilots table - " << query.lastError();
    }

    if(query.exec(strQueryRacesTable)){
        qDebug() << "RLTDatabase::initDatabase(): created races table";
    }else{
        qDebug() << "RLTDatabase::initDatabase(): failed to create races table - " << query.lastError();
    }

    if(query.exec(strQueryRacePilotsTable)){
        qDebug() << "RLTDatabase::initDatabase(): created race_pilots table";
    }else{
        qDebug() << "RLTDatabase::initDatabase(): failed to create race_pilots table - " << query.lastError();
    }

    if(query.exec(strQueryRaceLapsTable)){
        qDebug() << "RLTDatabase::initDatabase(): created race_laps table";
    }else{
        qDebug() << "RLTDatabase::initDatabase(): failed to create race_laps table - " << query.lastError();
    }

    return true;
}

void RLTDatabase::createDummyPilot(){
    QSqlQuery query(m_sqliteDatabase);
    QString strQueryDummyPilot = "INSERT INTO pilots(name,quad_token) VALUES('Dummy Pilot','1234');";

    query.exec(strQueryDummyPilot);
}

int RLTDatabase::createNewRace(QString v){
    QSqlQuery query(m_sqliteDatabase);
    QString strQueryDummyPilot = QString("INSERT INTO races(title,created_at) VALUES('%1','%2');").arg(v).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    query.exec(strQueryDummyPilot);

    if(!query.exec(strQueryDummyPilot)){
        qDebug() << "RLTDatabase::createNewRace(): failed to create new race - " << query.lastError();
    }else{
        qDebug() << "RLTDatabase::createNewRace(): created new race";
    }
    return query.lastInsertId().toInt();
}

/**
 * @brief adds  a pilot to a race, this method may be called multipe tims, it checks that each pilot for a race is unique
 * @param race_id
 * @param pilot_id
 * @return
 */
int RLTDatabase::addPilotToRace(int race_id,int pilot_id){
    qDebug() << QString("RLTDatabase::addPilotToRace(): pilot_id: %1  race_id %2").arg(pilot_id).arg(race_id);

    QSqlQuery query_check_pilot(m_sqliteDatabase);

    query_check_pilot.prepare("SELECT count(*) FROM race_pilots WHERE pilot_id= :pilot_id AND race_id= :race_id");
    query_check_pilot.bindValue(":pilot_id", pilot_id);
    query_check_pilot.bindValue(":race_id", race_id);

    query_check_pilot.exec();
    if(query_check_pilot.next()){
        //qDebug() << "count: " << query_check_pilot.value(0).toInt();

        if(query_check_pilot.value(0).toInt() == 0){
            // the pilot didnt get added to the race yet, so let's add him
            QSqlQuery query_add_pilot(m_sqliteDatabase);
            query_add_pilot.prepare("INSERT INTO race_pilots (race_id,pilot_id) VALUES(:race_id, :pilot_id);");
            query_add_pilot.bindValue(":race_id", race_id);
            query_add_pilot.bindValue(":pilot_id", pilot_id);
            if(query_add_pilot.exec()){
                qDebug() << QString("RLTDatabase::addPilotToRace(): added pilot %1 to race %2").arg(pilot_id).arg(race_id);
                return query_add_pilot.lastInsertId().toInt();
            }else{
                qDebug() << "RLTDatabase::addPilotToRace(): failed to add pilot to race " << query_add_pilot.lastError();
            }
        }
    }

    return 0;
}

int RLTDatabase::addLapTimeToRace(int race_id,int pilot_id,int lap_count,int lap_time){
    qDebug() << QString("RLTDatabase::addLapTimeToRace(): race_id: %1  pilot_id: %2 lap_number: %3 lap_time: %4").arg(race_id).arg(pilot_id).arg(lap_count).arg(lap_time);

    QSqlQuery query_check_pilot(m_sqliteDatabase);
    query_check_pilot.prepare("SELECT * FROM race_pilots WHERE pilot_id= :pilot_id AND race_id= :race_id");
    query_check_pilot.bindValue(":pilot_id", pilot_id);
    query_check_pilot.bindValue(":race_id", race_id);
    query_check_pilot.exec();

    if(query_check_pilot.next()){
        int race_pilot_id = query_check_pilot.value("id").toInt();

        QSqlQuery query_add_lap_to_race(m_sqliteDatabase);
        query_add_lap_to_race.prepare("INSERT INTO race_laps(race_id,race_pilot_id,lap_time,lap_count) VALUES(:race_id,:race_pilot_id,:lap_time,:lap_count);");
        query_add_lap_to_race.bindValue(":race_id",race_id);
        query_add_lap_to_race.bindValue(":race_pilot_id",race_pilot_id);
        query_add_lap_to_race.bindValue(":lap_time",lap_time);
        query_add_lap_to_race.bindValue(":lap_count",lap_count);

        if(query_add_lap_to_race.exec()){
            qDebug() << QString("RLTDatabase::addLapTimeToRace(): added lap to race");
            return query_check_pilot.lastInsertId().toInt();
        }
    }
    return 0;
}

QList<ModelRace*> RLTDatabase::getRaces(){
    QList<ModelRace*> data;

    QSqlQuery query_races(m_sqliteDatabase);
    query_races.prepare("SELECT * FROM races ORDER BY id DESC");
    query_races.exec();

    while(query_races.next()){
        ModelRace *race = new ModelRace();
        race->setId(query_races.value(0).toInt());
        race->setName(query_races.value(1).toString());
        race->setCreatedAt(query_races.value(2).toDateTime());
        data.append(race);
    }

    return data;
}
