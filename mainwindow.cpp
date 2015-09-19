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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "currentrace.h"
#include "qextserialport/qextserialenumerator.h"
#include <QInputDialog>
#include "racepilot.h"
#include "currentpilotracelap.h"
#include "settings.h"
#include <QFileDialog>
#include "racetablewidget.h"
#include "currentracewidget.h"
#include "pilotswidget.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include "configurationwidget.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("polyvision UG");
    QCoreApplication::setOrganizationDomain("polyvision.org");
    QCoreApplication::setApplicationName("RaceLapTimer");

    ui->setupUi(this);
    setWindowTitle(QString("OpenRaceLapTimer by airbirds.de - v%1").arg(RLT_VERSION));
    setWindowIcon(QIcon("app_icon.png"));

    QVBoxLayout *pLayout = new QVBoxLayout(ui->centralWidget);
    m_pTabWidget = new QTabWidget();
    pLayout->addWidget(m_pTabWidget);

    this->m_pRLTDatabase = RLTDatabase::instance();

    m_pConfigurationWidget = new ConfigurationWidget();
    m_pConfigurationWidget->setMainWindow(this);
    this->m_pTabWidget->addTab(m_pConfigurationWidget,"Configuration");

    m_pPilotsWidget = new PilotsWidget();
    this->m_pTabWidget->addTab(m_pPilotsWidget,"Pilots");

    m_pRaceTableWidget = new RaceTableWidget();
    this->m_pTabWidget->addTab(m_pRaceTableWidget,"Races");

     m_pCurrentRaceWidget = new CurrentRaceWidget();
     this->m_pTabWidget->addTab(m_pCurrentRaceWidget,"Current Race");

    this->m_pLabelCOMPortStatus = new QLabel(this);
    this->m_pLabelCOMPortStatus->setText("not connected to COM port");
    this->ui->statusBar->addWidget(m_pLabelCOMPortStatus);

    this->m_pLabelLastIncommingSignal = new QLabel(this);
    this->m_pLabelLastIncommingSignal->setText("XXX-XXX-XXX");
    this->ui->statusBar->addWidget(m_pLabelLastIncommingSignal);



    connect(CurrentRace::instance(),SIGNAL(pilotDataChanged()),this,SLOT(onCurrentRacePilotDataChanged()));
    connect(CurrentRace::instance(),SIGNAL(fastedLapChanged()),this,SLOT(onCurrentRaceFastestLapDataChanged()));
    connect(CurrentRace::instance(),SIGNAL(raceFinished()),this,SLOT(onCurentRaceFinished()));
    connect(m_pRaceTableWidget,SIGNAL(signalStartRace()),this,SLOT(onStartRaceClicked()));

    connect(m_pCurrentRaceWidget,SIGNAL(signalStopRace()),this,SLOT(onStopRaceClicked()));
    connect(m_pCurrentRaceWidget,SIGNAL(signalSimulate(QString)),this,SLOT(onSimulateClicked(QString)));

    connect(this->ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAboutDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAboutDialog(){
    AboutDialog *dialog = new AboutDialog(this);
    dialog->showNormal();
}

void MainWindow::onSimulateClicked(QString v)
{
    CurrentRace::instance()->incommingPilotSignal(v);
}

void MainWindow::connectCOM(QString v){
    this->m_pSerialPort = new QextSerialPort(v, QextSerialPort::EventDriven);
    m_pSerialPort->setBaudRate(BAUD9600);
    m_pSerialPort->setFlowControl(FLOW_OFF);
    m_pSerialPort->setParity(PAR_NONE);
    m_pSerialPort->setDataBits(DATA_8);
    m_pSerialPort->setStopBits(STOP_2);

    if (m_pSerialPort->open(QIODevice::ReadWrite) == true) {
        connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(m_pSerialPort, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(m_pSerialPort->lineStatus() & LS_DSR))
            qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << m_pSerialPort->portName();

        this->m_pLabelCOMPortStatus->setText(QString("connected to %1").arg(v));
    }
    else {
        qDebug() << "device failed to open:" << m_pSerialPort->errorString();
    }
}

void MainWindow::onReadyRead()
{
    QByteArray bytes;
    int a = this->m_pSerialPort->bytesAvailable();
    bytes.resize(a);
    this->m_pSerialPort->read(bytes.data(), bytes.size());
    //qDebug() << "bytes read:" << bytes.size();
    //qDebug() << "bytes:" << bytes;
    m_strIncommingSerialData = QString(bytes);

    int index = m_strIncommingSerialData.indexOf("#");
    if(index >= 0){
        //qDebug() << "before" << m_strIncommingSerialData;
        m_strIncommingSerialData.chop(m_strIncommingSerialData.length() - index -1);
        //qDebug() << "after" << m_strIncommingSerialData;
        this->m_pLabelLastIncommingSignal->setText(m_strIncommingSerialData);
        CurrentRace::instance()->incommingPilotSignal(m_strIncommingSerialData);
    }
}

void MainWindow::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}

void MainWindow::onStartRaceClicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Start a new race"),
                                         tr("race title:"), QLineEdit::Normal,
                                         NULL, &ok);
    if (ok && !text.isEmpty()){
        int race_id = RLTDatabase::instance()->createNewRace(text);
        CurrentRace::instance()->startRace(race_id,this);
        this->m_pTabWidget->setCurrentIndex(3);
    }

}

void MainWindow::setCurrentRaceTitle(QString v){
    m_pCurrentRaceWidget->setRaceTitle(v);
}

void MainWindow::onCurrentRacePilotDataChanged(){
    m_pCurrentRaceWidget->updateRaceData();
}

void MainWindow::onCurentRaceFinished(){
    this->m_pCurrentRaceWidget->raceStopped();
}

void MainWindow::onCurrentRaceFastestLapDataChanged(){
    // updating fastest pilot
    RacePilot *fastestPilot = CurrentRace::instance()->getFastestPilot();
    if(fastestPilot){
        m_pCurrentRaceWidget->setFastestLapData(QString("Pilot: %1 Lap: %2 Time: %3").arg(fastestPilot->getPilotName()).arg(fastestPilot->getFastedLap()->getLap()).arg(fastestPilot->getFastedLap()->formatedLapTime()));
    }
}

void MainWindow::onStopRaceClicked()
{
    CurrentRace::instance()->stopRace();
    m_pRaceTableWidget->updateTable();
    this->m_pTabWidget->setCurrentIndex(2);
}
