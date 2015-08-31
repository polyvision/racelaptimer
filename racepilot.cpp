#include "racepilot.h"
#include <QDebug>
#include <QSqlQuery>
#include "currentpilotracelap.h"
#include <QDebug>

RacePilot::RacePilot(QObject *parent) : QObject(parent)
{
    m_pCurrentRaceLap = NULL;
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
}

void RacePilot::finishLap(){
    this->m_pCurrentRaceLap->finishLap();

    qDebug() << QString("RacePilot::fireLapTime: %1 finished lap %2 on %3").arg(this->getPilotName()).arg(this->m_listLaps.size()).arg(this->m_pCurrentRaceLap->getLapEnd().toString());
    // let's start a new lap
    this->startLap();
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
