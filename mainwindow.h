#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rltdatabase.h"
#include "qextserialport/qextserialport.h"

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
    void on_buttonSimulatorLog_clicked();

    void on_buttonAddPilot_clicked();
    void on_buttonConnectSerialPort_clicked();

    void onReadyRead();
    void onDsrChanged(bool status);
    void on_buttonStartRace_clicked();

    void onCurrentRacePilotDataChanged();
    void onCurrentRaceFastestLapDataChanged();
    void on_buttonChangeLapBeepPath_clicked();

    void on_buttonChangeFastestLapShout_clicked();

private:
    void    setupCOMPortGUI();
    Ui::MainWindow *ui;
    RLTDatabase *m_pRLTDatabase;
    QSqlTableModel *m_pPilotsModel;
    QSqlTableModel *m_pRacesModel;
    QextSerialPort *m_pSerialPort;
};

#endif // MAINWINDOW_H
