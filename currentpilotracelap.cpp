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
