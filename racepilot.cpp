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

#include "racepilot.h"
#include <QDebug>
#include <QSqlQuery>
#include "currentpilotracelap.h"
#include <QDebug>
#include "rltdatabase.h"
#include "sfx.h"
#include "settings.h"

RacePilot::RacePilot(QObject *parent) : QObject(parent)
{
    m_pCurrentRaceLap = NULL;
    m_iCurrentRaceID = 0;
}


void RacePilot::setQuadToken(QString v){
    this->m_strQuadToken = v;
}

void RacePilot::setPilotName(QString v){
    this->m_strPilotName = v;
}

void RacePilot::setDatabaseID(QString v){
    this->m_strDatabaseID =v;
}

RacePilot* RacePilot::getByQuadToken(QString token){
    QSqlQuery query;
    query.prepare("SELECT * FROM pilots WHERE quad_token=:quad_token;");
    query.bindValue(":quad_token", token);
    query.exec();

    if(query.next()){
        RacePilot *t = new RacePilot();
        t->setPilotName(query.value("name").toString());
        t->setQuadToken(token);
        t->setDatabaseID(query.value("id").toString());
        return t;
    }else{
        //qDebug() << "RacePilot::initDatabase(): failed to find pilot -" << query.lastError();
    }

    return NULL;
}

int RacePilot::lapCount(){
    return this->m_listLaps.size();
}

QDateTime RacePilot::currentLapStartTime(){
    if(this->m_pCurrentRaceLap){
        return m_pCurrentRaceLap->getLapStart();
    }
    return QDateTime();
}

QString RacePilot::getQuadToken(){
    return m_strQuadToken;
}

QString RacePilot::getPilotName(){
    return m_strPilotName;
}

void RacePilot::fireLapTime(){
    // beginning of the race for this pilot
    if(this->m_pCurrentRaceLap == NULL){
        this->startLap();
    }else{
        this->finishLap();
    }
}

void RacePilot::startLap(){
    this->m_pCurrentRaceLap = new CurrentPilotRaceLap(this->m_listLaps.size() + 1);
    this->m_pCurrentRaceLap->startLap();
    this->m_listLaps << this->m_pCurrentRaceLap;

    qDebug() << QString("RacePilot::fireLapTime: %1 started lap %2 on %3").arg(this->getPilotName()).arg(this->m_listLaps.size()).arg(this->m_pCurrentRaceLap->getLapStart().toString());
    SFX::instance()->playBeep();
}

void RacePilot::finishLap(){
    // one lap can never be faster than  5 seconds
    QDateTime now = QDateTime::currentDateTime();
    if(this->m_pCurrentRaceLap->getLapStart().msecsTo(now) < Settings::instance()->getTrackingTimeout()){
        return;
    }

    if(this->m_pCurrentRaceLap->finishLap()){
        qDebug() << QString("RacePilot::fireLapTime: %1 finished lap %2 on %3").arg(this->getPilotName()).arg(this->m_listLaps.size()).arg(this->m_pCurrentRaceLap->getLapEnd().toString());

        // log the lap to the database
        RLTDatabase::instance()->addLapTimeToRace(m_iCurrentRaceID,m_strDatabaseID.toInt(),m_pCurrentRaceLap->getLap(),m_pCurrentRaceLap->lapTime());
        // let's start a new lap
        this->startLap();
    }
}

QString RacePilot::lastLapTimeString(){
    CurrentPilotRaceLap * lap = this->m_listLaps.at(this->m_listLaps.size() -2);
    if(lap){
        return lap->formatedLapTime();
    }
    return QString("undefined");
}

bool RacePilot::hasLastLapTime(){
    if(this->m_listLaps.size() >= 2){
        return true;
    }
    return false;
}

CurrentPilotRaceLap* RacePilot::getFastedLap(){
    CurrentPilotRaceLap *fastestLap = NULL;

    for(int i = 0; i < this->m_listLaps.size(); i++){
        CurrentPilotRaceLap *lap = this->m_listLaps.at(i);

        if(lap->isFinished()){
            if(fastestLap && fastestLap->lapTime() > lap->lapTime()){
                fastestLap = lap;
            }else if(fastestLap == NULL){
                fastestLap = lap;
            }
        }
    }

    return fastestLap;
}

int RacePilot::getID(){
    return m_strDatabaseID.toInt();
}

void RacePilot::setCurrentRaceID(int id){
    m_iCurrentRaceID = id;
}
