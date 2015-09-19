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

#include "configurationwidget.h"
#include <QGroupBox>
#include "qextserialport/qextserialenumerator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QDebug>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>
#include "settings.h"
#include "qfiledialog.h"
#include <QLineEdit>

ConfigurationWidget::ConfigurationWidget(QWidget *parent) : QWidget(parent)
{
    m_pLayout = new QVBoxLayout(this);

    QHBoxLayout *firstLineHBox = new QHBoxLayout();
    m_pLayout->addLayout(firstLineHBox);

    // COM
    QGroupBox *pCOMGroupBox = new QGroupBox();
    pCOMGroupBox->setTitle("COM-Port for incomming data");
    firstLineHBox->addWidget(pCOMGroupBox);

    QVBoxLayout *pLayoutCOMGroupBox = new QVBoxLayout(pCOMGroupBox);
    m_pComboCOMPortSelection = new QComboBox();
    pLayoutCOMGroupBox->addWidget(m_pComboCOMPortSelection);

    m_pButtonConnectCOMPort = new QPushButton();
    m_pButtonConnectCOMPort->setText("connect");
    pLayoutCOMGroupBox->addWidget(m_pButtonConnectCOMPort);

    // general settings
    QGroupBox *pGeneralGroupBox = new QGroupBox();
    pGeneralGroupBox->setTitle("general settings");
    firstLineHBox->addWidget(pGeneralGroupBox);

    QVBoxLayout *pGeneralGroupBoxLayout = new QVBoxLayout(pGeneralGroupBox);
    this->m_pLabelTrackingTimeout = new QLabel();
    this->m_pLabelTrackingTimeout->setText("Tracking timeout (ms)");
    pGeneralGroupBoxLayout->addWidget(m_pLabelTrackingTimeout);

    this->m_pLineEditTrackingTimeout = new QLineEdit();
    m_pLineEditTrackingTimeout->setText(QString("%1").arg(Settings::instance()->getTrackingTimeout()));
    pGeneralGroupBoxLayout->addWidget(m_pLineEditTrackingTimeout);



    // SFX
    QGroupBox *pSFXGroupBox = new QGroupBox();
    pSFXGroupBox->setTitle("Sound Effects");
    m_pLayout->addWidget(pSFXGroupBox);

    QVBoxLayout *pSFXGroupBoxLayout = new QVBoxLayout(pSFXGroupBox);

    m_pLabelSFXBeep = new QLabel();
    m_pLabelSFXBeep->setText(QString("beep: %1").arg(Settings::instance()->getLapBeepPath()));
    pSFXGroupBoxLayout->addWidget(m_pLabelSFXBeep);

    m_pButtonSFXBeep = new QPushButton();
    m_pButtonSFXBeep->setText("change path for beep");
    pSFXGroupBoxLayout->addWidget(m_pButtonSFXBeep);

    m_pLabelSFXFastestLap = new QLabel();
    this->m_pLabelSFXFastestLap->setText(QString("fastest lap: %1").arg(Settings::instance()->getFastestLapSoundPath()));
    pSFXGroupBoxLayout->addWidget(m_pLabelSFXFastestLap);

    m_pButtonSFXFastestLap = new QPushButton();
    m_pButtonSFXFastestLap->setText("change path for fastest lap");
    pSFXGroupBoxLayout->addWidget(m_pButtonSFXFastestLap);


    this->setupComPorts();

    connect(m_pButtonConnectCOMPort,SIGNAL(clicked(bool)),this,SLOT(buttonConnectCOMPortClicked(bool)));
    connect(m_pButtonSFXBeep,SIGNAL(clicked(bool)),this,SLOT(buttonSFXBeepClicked(bool)));
    connect(m_pButtonSFXFastestLap,SIGNAL(clicked(bool)),this,SLOT(buttonSFXFastestLapClicked(bool)));
    connect(m_pLineEditTrackingTimeout,SIGNAL(textChanged(QString)),this,SLOT(lineEditTimeoutChanged(QString)));
}

void ConfigurationWidget::setMainWindow(MainWindow* v){
    m_pMainWindow = v;
}

void ConfigurationWidget::setupComPorts(){
    QList<QextPortInfo> portList = QextSerialEnumerator::getPorts();
    for(int i = 0; i < portList.size(); i++){
        m_pComboCOMPortSelection->addItem(QString("%1 - %2").arg(portList.at(i).portName).arg(portList.at(i).friendName));
    }
}

void ConfigurationWidget::buttonConnectCOMPortClicked(bool)
{

    QList<QextPortInfo> portList = QextSerialEnumerator::getPorts();

    m_pMainWindow->connectCOM(portList.at(m_pComboCOMPortSelection->currentIndex()).portName);
}

void ConfigurationWidget::buttonSFXBeepClicked(bool){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select WAV"), NULL, tr("sound files (*.wav)"));
    if(!fileName.isEmpty()){
        Settings::instance()->setLapBeepPath(fileName);
        m_pLabelSFXBeep->setText(QString("beep: %1").arg(Settings::instance()->getLapBeepPath()));
    }
}

void ConfigurationWidget::buttonSFXFastestLapClicked(bool){

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select WAV"), NULL, tr("sound files (*.wav)"));
    if(!fileName.isEmpty()){
        Settings::instance()->setFastestLapSoundPath(fileName);
        m_pLabelSFXFastestLap->setText(QString("fastest lap: %1").arg(Settings::instance()->getFastestLapSoundPath()));
    }
}

void ConfigurationWidget::lineEditTimeoutChanged(QString v){
    Settings::instance()->setTrackingTimeout(v.toInt());
}
