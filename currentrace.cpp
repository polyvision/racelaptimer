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
}


void CurrentRace::incommingPilotSignal(QString token){
    //is there a running race?
    if(m_uiRaceState == CR_RACE_STATE_NOT_RUNNING){
        return;
    }

    RacePilot *pilot = this->getPilotByToken(token);

    // add the pilot to the race list or add it if it can be found
    if(pilot == NULL){
        pilot = RacePilot::getByQuadToken(token);
        if(pilot){
            this->m_listPilots << pilot;
            qDebug() << QString("CurrentRace::incommingPilotSignal: added pilot to current race list count %1").arg(this->m_listPilots.size());
        }
    }else{
        qDebug() << "CurrentRace::incommingPilotSignal: pilot found in current race list";
    }

    if(pilot){
        pilot->fireLapTime();
        SFX::instance()->playBeep();
    }
}

RacePilot* CurrentRace::getPilotByToken(QString token){
    for (int i = 0; i < m_listPilots.size(); ++i) {
        if (m_listPilots.at(i)->getQuadToken().compare(token) == 0){
            return m_listPilots.at(i);
        }
    }
    return NULL;
}

void CurrentRace::updateTableWidget(QTableWidget *widget,QLabel *fastestLapLabel){
    widget->clearContents();
    widget->setColumnCount(5);
    widget->setRowCount(m_listPilots.size());
    widget->setHorizontalHeaderItem(0,new QTableWidgetItem("Pilot"));
    widget->setHorizontalHeaderItem(1,new QTableWidgetItem("Laps"));
    widget->setHorizontalHeaderItem(2,new QTableWidgetItem("Last Lap Time"));
    widget->setHorizontalHeaderItem(3,new QTableWidgetItem("Fasted Lap"));
    widget->setHorizontalHeaderItem(4,new QTableWidgetItem("Fasted Lap Time"));

    for (int i = 0; i < m_listPilots.size(); ++i) {

        RacePilot *pilot = m_listPilots.at(i);

        // pilot name
        QTableWidgetItem *itemPilotName = new QTableWidgetItem(pilot->getPilotName());
        widget->setItem(i,0,itemPilotName);

        // lap count
        QTableWidgetItem *itemLapCount = new QTableWidgetItem(QString("%1").arg(pilot->lapCount()));
        widget->setItem(i,1,itemLapCount);

        // last lap time
        if(pilot->hasLastLapTime()){
            QTableWidgetItem *itemLastLapTime = new QTableWidgetItem(QString("%1").arg(pilot->lastLapTimeString()));
            widget->setItem(i,2,itemLastLapTime);
        }

        // fastest lap // fastest lap time
        if(pilot->getFastedLap()){
            QTableWidgetItem *itemFastestLap = new QTableWidgetItem(QString("%1").arg(pilot->getFastedLap()->getLap()));
            widget->setItem(i,3,itemFastestLap);

            QTableWidgetItem *itemFastestLapTime = new QTableWidgetItem(QString("%1").arg(pilot->getFastedLap()->formatedLapTime()));
            widget->setItem(i,4,itemFastestLapTime);
        }

    }

    // updating fastest pilot
    RacePilot *fastestPilot = this->getFastestPilot();
    if(fastestPilot){
        fastestLapLabel->setText(QString("Pilot: %1 Lap: %2 Time:%3").arg(fastestPilot->getPilotName()).arg(fastestPilot->getFastedLap()->getLap()).arg(fastestPilot->getFastedLap()->formatedLapTime()));
    }
}

/*
 * starts a new race, needs the race_id which got created former for the race
 */
void CurrentRace::startRace(int race_id,MainWindow *window){
    this->m_pModelRace = new QSqlQueryModel();
    this->m_pModelRace->setQuery(QString("SELECT * FROM races WHERE id=%1").arg(race_id));
    window->setCurrentRaceTitle(this->m_pModelRace->record(0).value("title").toString());

    m_uiRaceState = CR_RACE_STATE_RUNNING;
}

void CurrentRace::stopRace(){
    m_uiRaceState = CR_RACE_STATE_NOT_RUNNING;
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

    return fastestPilot;
}
