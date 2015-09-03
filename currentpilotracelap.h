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
