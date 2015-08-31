#ifndef CURRENTRACE_H
#define CURRENTRACE_H

#include <QObject>
#include <QList>
#include "singleton.h"

#define CR_RACE_STATE_NOT_RUNNING   0
#define CR_RACE_STATE_RUNNING   1

class RacePilot;
class QTableWidget;
class QSqlQueryModel;
class MainWindow;
class QLabel;

class CurrentRace : public QObject,public Singleton<CurrentRace>
{
    Q_OBJECT
    friend class Singleton<CurrentRace>;

public:
    explicit CurrentRace(QObject *parent = 0);
    void    incommingPilotSignal(QString);

    void    updateTableWidget(QTableWidget*,QLabel*);
    void    startRace(int,MainWindow*);
    void    stopRace();

    RacePilot* getFastestPilot();
signals:

public slots:
private:
    unsigned int     m_uiRaceState;
    QSqlQueryModel *m_pModelRace;
    RacePilot*  getPilotByToken(QString);
    QList<RacePilot*> m_listPilots;
};

#endif // CURRENTRACE_H
