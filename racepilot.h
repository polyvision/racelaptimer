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
    int         lapCount();
    QDateTime   currentLapStartTime();
    QString     lastLapTimeString();
    bool        hasLastLapTime();
    CurrentPilotRaceLap* getFastedLap();
    void        fireLapTime();

    static RacePilot* getByQuadToken(QString);
signals:

public slots:

private:
    void    startLap();
    void    finishLap();

    QString m_strQuadToken;
    QString m_strPilotName;
    QString m_strDatabaseID;

    QList<CurrentPilotRaceLap*> m_listLaps;
    CurrentPilotRaceLap*    m_pCurrentRaceLap;
};

#endif // RACEPILOT_H
