#include "racetablewidget.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include "rltdatabase.h"
#include "modelrace.h"
#include <QDebug>
#include <QPushButton>

RaceTableWidget::RaceTableWidget(QWidget *parent) : QWidget(parent)
{
    m_pLayout= new QVBoxLayout(this);
    m_pTableRaces = new QTableWidget(this);

    m_pLayout->addWidget(m_pTableRaces);

    m_pTableRaces->setColumnCount(5);
    //this->ui->tableWidget->setRowCount(CurrentRace::instance()->getPilotsList()->size());
    m_pTableRaces->setHorizontalHeaderItem(0,new QTableWidgetItem("name"));
    m_pTableRaces->setHorizontalHeaderItem(1,new QTableWidgetItem("created at"));
    m_pTableRaces->setHorizontalHeaderItem(2,new QTableWidgetItem("num pilots"));
    m_pTableRaces->setHorizontalHeaderItem(3,new QTableWidgetItem("fastest lap"));
    m_pTableRaces->setHorizontalHeaderItem(4,new QTableWidgetItem("fastest pilot"));


    m_pButtonStartRace = new QPushButton();
    m_pButtonStartRace->setText("start a new race");
    this->m_pLayout->addWidget(m_pButtonStartRace);

    this->updateTable();

    connect(m_pButtonStartRace,SIGNAL(clicked(bool)),this,SLOT(startButtonClicked(bool)));
   // pilot name

}

void RaceTableWidget::updateTable(){
    QList<ModelRace*> list = RLTDatabase::instance()->getRaces();

    m_pTableRaces->clearContents();
    m_pTableRaces->setRowCount(list.size());

    for(int i = 0; i < list.size(); i++){
        ModelRace *model = list.at(i);

        QTableWidgetItem *itemPilotName = new QTableWidgetItem(model->getName());
        m_pTableRaces->setItem(i,0,itemPilotName);

        QTableWidgetItem *itemCreatedAt = new QTableWidgetItem(model->getCreatedAt().toString());
        m_pTableRaces->setItem(i,1,itemCreatedAt);

        QTableWidgetItem *itemNumPilots = new QTableWidgetItem(QString("%1").arg(model->getNumPilots()));
        m_pTableRaces->setItem(i,2,itemNumPilots);

        QTableWidgetItem *itemFastestLapTime = new QTableWidgetItem(model->getFormatedFastedLapTime());
        m_pTableRaces->setItem(i,3,itemFastestLapTime);

        QTableWidgetItem *itemFastestLapTimePilot = new QTableWidgetItem(model->getFastestPilotName());
        m_pTableRaces->setItem(i,4,itemFastestLapTimePilot);
    }
}

void RaceTableWidget::startButtonClicked(bool){
    emit signalStartRace();
}
