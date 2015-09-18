/**
 * OpenRaceLapTimer - Copyright 2015 by airbirds.de, a project of polyvision UG (haftungsbeschränkt)
 *
 * Author: Alexander B. Bierbrauer
 *
 * This file is part of OpenRaceLapTimer.
 *
 * OpenRaceLapTimer is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * OpenRaceLapTimer is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Foobar. If not, see http://www.gnu.org/licenses/.
 **/

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
