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

#ifndef CURRENTRACEWIDGET_H
#define CURRENTRACEWIDGET_H

#include <QWidget>

class QTableWidget;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class CurrentRaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CurrentRaceWidget(QWidget *parent = 0);

    void setRaceTitle(QString);
    void setFastestLapData(QString);
    void updateRaceData();
    void raceStopped();

signals:
    void  signalStopRace();
    void  signalSimulate(QString);

public slots:
    void        clickedStopRace(bool);
    void        clickedSimulate(bool);
private:
    QTableWidget *m_pTableWidget;
    QVBoxLayout *m_pLayout;
    QLabel      *m_pCurrentRaceTitleLabel;
    QLabel       *m_pFastestLapData;
    QLineEdit   *m_pSimulatorToken;
    QPushButton *m_pButtonSimulator;
    QPushButton *m_pButtonStopRace;
};

#endif // CURRENTRACEWIDGET_H
