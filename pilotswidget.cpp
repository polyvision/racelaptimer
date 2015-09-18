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

#include "pilotswidget.h"
#include "rltdatabase.h"
#include <QSqlDatabase>
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>

PilotsWidget::PilotsWidget(QWidget *parent) : QWidget(parent)
{
    m_pLayout = new QVBoxLayout(this);

    m_pButtonAddPilot = new QPushButton();
    m_pButtonAddPilot->setText("add pilot");
    m_pLayout->addWidget(m_pButtonAddPilot);

    m_pTableView = new QTableView(this);

    // pilots table
    m_pPilotsModel = new QSqlTableModel(this, *RLTDatabase::instance()->database());
    m_pPilotsModel->setTable("pilots");
    m_pPilotsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    m_pPilotsModel->select();
    m_pPilotsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_pPilotsModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_pPilotsModel->setHeaderData(2, Qt::Horizontal, tr("API-Token"));
    m_pPilotsModel->setHeaderData(3, Qt::Horizontal, tr("Quad-Token"));

    this->m_pTableView->setModel(m_pPilotsModel);
    this->m_pTableView->hideColumn(0); // don't show the ID*/

    m_pLayout->addWidget(m_pTableView);

    connect(m_pButtonAddPilot,SIGNAL(clicked(bool)),this,SLOT(clickedAddPilot(bool)));
}

void PilotsWidget::clickedAddPilot(bool){
    RLTDatabase::instance()->createDummyPilot();
    m_pPilotsModel->select();
}
