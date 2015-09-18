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

#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QObject>
#include <QWidget>

class QVBoxLayout;
class QPushButton;
class QComboBox;
class MainWindow;
class QLabel;

class ConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigurationWidget(QWidget *parent = 0);
    void    setMainWindow(MainWindow*);
signals:

public slots:
    void    buttonConnectCOMPortClicked(bool);
    void    buttonSFXBeepClicked(bool);
    void    buttonSFXFastestLapClicked(bool);

private:
    void            setupComPorts();

    QVBoxLayout     *m_pLayout;
    QComboBox       *m_pComboCOMPortSelection;
    QPushButton     *m_pButtonConnectCOMPort;
    MainWindow      *m_pMainWindow;

    QLabel          *m_pLabelSFXBeep;
    QLabel          *m_pLabelSFXFastestLap;
    QPushButton     *m_pButtonSFXBeep;
    QPushButton     *m_pButtonSFXFastestLap;
};

#endif // CONFIGURATIONWIDGET_H
