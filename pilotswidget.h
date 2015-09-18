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

#ifndef PILOTSWIDGET_H
#define PILOTSWIDGET_H

#include <QObject>
#include <QWidget>

class QTableView;
class QVBoxLayout;
class QPushButton;
class QSqlTableModel;

class PilotsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PilotsWidget(QWidget *parent = 0);

signals:

public slots:
    void    clickedAddPilot(bool);

private:
    QTableView      *m_pTableView;
    QVBoxLayout     *m_pLayout;
    QPushButton     *m_pButtonAddPilot;
    QSqlTableModel  *m_pPilotsModel;

};

#endif // PILOTSWIDGET_H
