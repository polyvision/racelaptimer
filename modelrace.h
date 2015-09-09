#ifndef MODELRACE_H
#define MODELRACE_H


#include <QString>
#include <QDateTime>

class ModelRace
{
public:
    ModelRace();

    int getId();
    QString getName();
    QDateTime getCreatedAt();

    void setId(int);
    void setName(QString);
    void setCreatedAt(QDateTime);

    int getNumPilots();
    QString getFormatedFastedLapTime();
    QString getFastestPilotName();

private:
    int     m_Id;
    QString m_Name;
    QDateTime m_Created_at;
};

#endif // MODELRACE_H
