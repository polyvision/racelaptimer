#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rltdatabase.h"
#include "qextserialport/qextserialport.h"
#include <QLabel>

#define RLT_VERSION "0.0.2"

class RaceTableWidget;
class CurrentRaceWidget;

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
private slots:
    void on_buttonAddPilot_clicked();
    void on_buttonConnectSerialPort_clicked();

    void onReadyRead();
    void onDsrChanged(bool status);
    void onStartRaceClicked();

    void onCurrentRacePilotDataChanged();
    void onCurrentRaceFastestLapDataChanged();
    void onCurentRaceFinished();
    void on_buttonChangeLapBeepPath_clicked();

    void on_buttonChangeFastestLapShout_clicked();

    void onStopRaceClicked();
    void onSimulateClicked(QString);

private:
    void    setupCOMPortGUI();
    Ui::MainWindow *ui;
    RLTDatabase *m_pRLTDatabase;
    QSqlTableModel *m_pPilotsModel;
    QSqlTableModel *m_pRacesModel;
    QextSerialPort *m_pSerialPort;
    QLabel          *m_pLabelCOMPortStatus;
    QLabel          *m_pLabelLastIncommingSignal;
    QString         m_strIncommingSerialData;
    RaceTableWidget *m_pRaceTableWidget;
    CurrentRaceWidget   *m_pCurrentRaceWidget;
};

#endif // MAINWINDOW_H
