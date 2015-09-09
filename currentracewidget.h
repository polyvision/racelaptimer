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
