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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rltdatabase.h"
#include "qextserialport/qextserialport.h"
#include <QLabel>

#define RLT_VERSION "0.0.3"

class RaceTableWidget;
class CurrentRaceWidget;
class PilotsWidget;
class QTabWidget;
class ConfigurationWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setCurrentRaceTitle(QString);
    void connectCOM(QString);

private slots:

    void onReadyRead();
    void onDsrChanged(bool status);
    void onStartRaceClicked();

    void onCurrentRacePilotDataChanged();
    void onCurrentRaceFastestLapDataChanged();
    void onCurentRaceFinished();

    void onStopRaceClicked();
    void onSimulateClicked(QString);

    void openAboutDialog();
private:
    Ui::MainWindow      *ui;
    RLTDatabase         *m_pRLTDatabase;
    QSqlTableModel      *m_pPilotsModel;
    QSqlTableModel      *m_pRacesModel;
    QextSerialPort      *m_pSerialPort;
    QLabel              *m_pLabelCOMPortStatus;
    QLabel              *m_pLabelLastIncommingSignal;
    QString             m_strIncommingSerialData;
    RaceTableWidget     *m_pRaceTableWidget;
    CurrentRaceWidget   *m_pCurrentRaceWidget;
    PilotsWidget        *m_pPilotsWidget;
    QTabWidget          *m_pTabWidget;
    ConfigurationWidget *m_pConfigurationWidget;
};

#endif // MAINWINDOW_H
