#include "currentracewidget.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "currentrace.h"
#include "racepilot.h"
#include "currentpilotracelap.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

CurrentRaceWidget::CurrentRaceWidget(QWidget *parent) : QWidget(parent)
{
    m_pLayout = new QVBoxLayout(this);

    m_pCurrentRaceTitleLabel = new QLabel();
    m_pCurrentRaceTitleLabel->setAlignment(Qt::AlignCenter);
    m_pLayout->addWidget(m_pCurrentRaceTitleLabel);

    m_pFastestLapData = new QLabel();
    m_pFastestLapData->setAlignment(Qt::AlignCenter);
    m_pLayout->addWidget(m_pFastestLapData);

    m_pTableWidget = new QTableWidget(this);
    m_pLayout->addWidget(m_pTableWidget);

    QHBoxLayout *hLayout = new QHBoxLayout();
    m_pLayout->addLayout(hLayout);

    m_pSimulatorToken = new QLineEdit();

    m_pButtonSimulator = new QPushButton();
    m_pButtonSimulator->setText("simulate");

    m_pButtonStopRace = new QPushButton();
    m_pButtonStopRace->setText("stop race");

    hLayout->addWidget(m_pButtonStopRace);
    hLayout->addWidget(m_pSimulatorToken);
    hLayout->addWidget(m_pButtonSimulator);

    connect(m_pButtonStopRace,SIGNAL(clicked(bool)),this,SLOT(clickedStopRace(bool)));
    connect(m_pButtonSimulator,SIGNAL(clicked(bool)),this,SLOT(clickedSimulate(bool)));
}

void CurrentRaceWidget::setRaceTitle(QString v){
    m_pCurrentRaceTitleLabel->setText(v);
}

void CurrentRaceWidget::setFastestLapData(QString v){
    m_pFastestLapData->setText(v);
}

void CurrentRaceWidget::updateRaceData(){
    m_pTableWidget->clearContents();
    m_pTableWidget->setColumnCount(5);
    m_pTableWidget->setRowCount(CurrentRace::instance()->getPilotsList()->size());
    m_pTableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Pilot"));
    m_pTableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Laps"));
    m_pTableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Last Lap Time"));
    m_pTableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Fasted Lap"));
    m_pTableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Fasted Lap Time"));

    for (int i = 0; i < CurrentRace::instance()->getPilotsList()->size(); ++i) {

        RacePilot *pilot = CurrentRace::instance()->getPilotsList()->at(i);

        // pilot name
        QTableWidgetItem *itemPilotName = new QTableWidgetItem(pilot->getPilotName());
        m_pTableWidget->setItem(i,0,itemPilotName);

        // lap count
        QTableWidgetItem *itemLapCount = new QTableWidgetItem(QString("%1").arg(pilot->lapCount()-1));
        m_pTableWidget->setItem(i,1,itemLapCount);

        // last lap time
        if(pilot->hasLastLapTime()){
            QTableWidgetItem *itemLastLapTime = new QTableWidgetItem(QString("%1").arg(pilot->lastLapTimeString()));
            m_pTableWidget->setItem(i,2,itemLastLapTime);
        }

        // fastest lap // fastest lap time
        if(pilot->getFastedLap()){
            QTableWidgetItem *itemFastestLap = new QTableWidgetItem(QString("%1").arg(pilot->getFastedLap()->getLap()));
            m_pTableWidget->setItem(i,3,itemFastestLap);

            QTableWidgetItem *itemFastestLapTime = new QTableWidgetItem(QString("%1").arg(pilot->getFastedLap()->formatedLapTime()));
            m_pTableWidget->setItem(i,4,itemFastestLapTime);
        }
    }
}

void CurrentRaceWidget::clickedStopRace(bool){
    emit  signalStopRace();
}

void CurrentRaceWidget::clickedSimulate(bool){
    emit  signalSimulate(m_pSimulatorToken->text());
}

void CurrentRaceWidget::raceStopped(){
    this->setRaceTitle("");
    this->setFastestLapData("");
    this->m_pTableWidget->clearContents();
    this->updateRaceData();
}
