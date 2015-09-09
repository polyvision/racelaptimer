#ifndef RACETABLEWIDGET_H
#define RACETABLEWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QTableWidget;
class QPushButton;

class RaceTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RaceTableWidget(QWidget *parent = 0);
    void    updateTable();

signals:
    void    signalStartRace();

public slots:
    void    startButtonClicked(bool);

private:


    QVBoxLayout *m_pLayout;
    QTableWidget *m_pTableRaces;
    QPushButton *m_pButtonStartRace;
};

#endif // RACETABLEWIDGET_H
