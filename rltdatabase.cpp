#include "rltdatabase.h"
#include <QDebug>

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

QSqlDatabase RLTDatabase::database(){
    return m_sqliteDatabase;
}

bool RLTDatabase::initDatabase(){
    QSqlQuery query(m_sqliteDatabase);
    QString strQueryPilotsTable = "CREATE TABLE pilots(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT NOT NULL,api_token CHAR(50),quad_token CHAR(50));";
    QString strQueryRacesTable = "CREATE TABLE races(id INTEGER PRIMARY KEY AUTOINCREMENT,title TEXT NOT NULL,created_at TEXT NOT NULL);";
    QString strQueryRacePilotsTable = "CREATE TABLE race_pilots(id INTEGER PRIMARY KEY AUTOINCREMENT,pilot_id INTEGER);";
    QString strQueryRaceLapsTable = "CREATE TABLE race_laps(id INTEGER PRIMARY KEY AUTOINCREMENT,race_pilot_id INTEGER,lap_time INTEGER,lap_count INTEGER);";

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
