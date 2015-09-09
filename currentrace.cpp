#include "currentrace.h"
#include "racepilot.h"
#include <QDebug>
#include <QTableWidget>
#include "sfx.h"
#include <QSqlQueryModel>
#include "mainwindow.h"
#include "currentpilotracelap.h"
#include <QLabel>

CurrentRace::CurrentRace(QObject *parent) : QObject(parent)
{
    this->m_pModelRace = NULL;
    m_uiRaceState = CR_RACE_STATE_NOT_RUNNING;
    m_iFastestLapTime = 0;
}


void CurrentRace::incommingPilotSignal(QString token){
    //is there a running race?
    if(m_uiRaceState == CR_RACE_STATE_NOT_RUNNING){
        return;
    }

    qDebug() << QString("CurrentRace::incommingPilotSignal: %1").arg(token);
    RacePilot *pilot = this->getPilotByToken(token);

    // add the pilot to the race list or add it if it can be found
    if(pilot == NULL){
        pilot = RacePilot::getByQuadToken(token);
        if(pilot){
            pilot->setCurrentRaceID(this->m_pModelRace->record(0).value("id").toInt()); // setting the current race id

            this->m_listPilots << pilot;
            RLTDatabase::instance()->addPilotToRace(this->m_pModelRace->record(0).value("id").toInt(),pilot->getID());
            qDebug() << QString("CurrentRace::incommingPilotSignal: added pilot to current race list count %1").arg(this->m_listPilots.size());
        }
    }else{
        qDebug() << "CurrentRace::incommingPilotSignal: pilot found in current race list";
    }

    if(pilot){
        pilot->fireLapTime();
    }

    emit pilotDataChanged();
    emit fastedLapChanged();
}

RacePilot* CurrentRace::getPilotByToken(QString token){
    for (int i = 0; i < m_listPilots.size(); ++i) {
        if (m_listPilots.at(i)->getQuadToken().compare(token) == 0){
            return m_listPilots.at(i);
        }
    }
    return NULL;
}

/*
 * starts a new race, needs the race_id which got created former for the race
 */
void CurrentRace::startRace(int race_id,MainWindow *window){
    this->m_pModelRace = new QSqlQueryModel();
    this->m_pModelRace->setQuery(QString("SELECT * FROM races WHERE id=%1").arg(race_id));
    window->setCurrentRaceTitle(this->m_pModelRace->record(0).value("title").toString());

    m_uiRaceState = CR_RACE_STATE_RUNNING;
    m_iFastestLapTime = 0;
}

void CurrentRace::stopRace(){
    m_uiRaceState = CR_RACE_STATE_NOT_RUNNING;
    this->m_listPilots.clear();
    emit raceFinished();
}

RacePilot* CurrentRace::getFastestPilot(){
    RacePilot *fastestPilot = NULL;

    for(int i = 0; i < this->m_listPilots.size(); i++){
        RacePilot *pilot = this->m_listPilots.at(i);

        if(fastestPilot == NULL && pilot != NULL && pilot->getFastedLap()){
            fastestPilot = pilot;
        }else if(pilot && fastestPilot){

          if(fastestPilot->getFastedLap() && pilot->getFastedLap()){
              if(fastestPilot->getFastedLap()->lapTime() > pilot->getFastedLap()->lapTime()){
                  fastestPilot = pilot;
              }
          }
        }
    }

    if(fastestPilot){
        if(m_iFastestLapTime == 0){
            m_iFastestLapTime = fastestPilot->getFastedLap()->lapTime();
            SFX::instance()->playFastestLap();
        }else{
            if(m_iFastestLapTime > fastestPilot->getFastedLap()->lapTime()){
                m_iFastestLapTime = fastestPilot->getFastedLap()->lapTime();
                SFX::instance()->playFastestLap();
            }
        }

    }
    return fastestPilot;
}

QList<RacePilot*>* CurrentRace::getPilotsList(){
    return &m_listPilots;
}
