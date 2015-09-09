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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle(QString("FPV-Racing Lap Timer by airbirds.de - %1").arg(RLT_VERSION));
    setWindowIcon(QIcon("app_icon.png"));
    QCoreApplication::setOrganizationName("polyvision UG");
    QCoreApplication::setOrganizationDomain("polyvision.org");
    QCoreApplication::setApplicationName("RaceLapTimer");

    ui->setupUi(this);
    this->m_pRLTDatabase = RLTDatabase::instance();

    m_pRaceTableWidget = new RaceTableWidget();
    this->ui->tabWidget->addTab(m_pRaceTableWidget,"Races");

     m_pCurrentRaceWidget = new CurrentRaceWidget();
     this->ui->tabWidget->addTab(m_pCurrentRaceWidget,"Current Race");

    // pilots table
    m_pPilotsModel = new QSqlTableModel(this, *this->m_pRLTDatabase->database());
    m_pPilotsModel->setTable("pilots");
    m_pPilotsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    m_pPilotsModel->select();
    m_pPilotsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_pPilotsModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_pPilotsModel->setHeaderData(2, Qt::Horizontal, tr("API-Token"));
    m_pPilotsModel->setHeaderData(3, Qt::Horizontal, tr("Quad-Token"));

    this->ui->m_uiTableViewPilots->setModel(m_pPilotsModel);
    this->ui->m_uiTableViewPilots->hideColumn(0); // don't show the ID

    this->setupCOMPortGUI();

    this->m_pLabelCOMPortStatus = new QLabel(this);
    this->m_pLabelCOMPortStatus->setText("not connected to COM port");
    this->ui->statusBar->addWidget(m_pLabelCOMPortStatus);

    this->m_pLabelLastIncommingSignal = new QLabel(this);
    this->m_pLabelLastIncommingSignal->setText("XXX-XXX-XXX");
    this->ui->statusBar->addWidget(m_pLabelLastIncommingSignal);


    // settings
    this->ui->labelSettingsLapBeep->setText(Settings::instance()->getLapBeepPath());
    this->ui->labelSettingsFastestLapShoutPath->setText(Settings::instance()->getFastestLapSoundPath());

    connect(CurrentRace::instance(),SIGNAL(pilotDataChanged()),this,SLOT(onCurrentRacePilotDataChanged()));
    connect(CurrentRace::instance(),SIGNAL(fastedLapChanged()),this,SLOT(onCurrentRaceFastestLapDataChanged()));
    connect(CurrentRace::instance(),SIGNAL(raceFinished()),this,SLOT(onCurentRaceFinished()));
    connect(m_pRaceTableWidget,SIGNAL(signalStartRace()),this,SLOT(onStartRaceClicked()));

    connect(m_pCurrentRaceWidget,SIGNAL(signalStopRace()),this,SLOT(onStopRaceClicked()));
    connect(m_pCurrentRaceWidget,SIGNAL(signalSimulate(QString)),this,SLOT(onSimulateClicked(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSimulateClicked(QString v)
{
    CurrentRace::instance()->incommingPilotSignal(v);
}

void MainWindow::on_buttonAddPilot_clicked()
{
    RLTDatabase::instance()->createDummyPilot();
    m_pPilotsModel->select();
}

void MainWindow::setupCOMPortGUI(){
    QList<QextPortInfo> portList = QextSerialEnumerator::getPorts();
    for(int i = 0; i < portList.size(); i++){
        this->ui->cb_COMPorts->addItem(QString("%1 - %2").arg(portList.at(i).portName).arg(portList.at(i).friendName));
    }
}

void MainWindow::on_buttonConnectSerialPort_clicked()
{
    QList<QextPortInfo> portList = QextSerialEnumerator::getPorts();

    this->m_pSerialPort = new QextSerialPort(portList.at(this->ui->cb_COMPorts->currentIndex()).portName, QextSerialPort::EventDriven);
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

        this->m_pLabelCOMPortStatus->setText(QString("connected to %1").arg(portList.at(this->ui->cb_COMPorts->currentIndex()).portName));
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
        this->ui->tabWidget->setCurrentIndex(3);
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

void MainWindow::on_buttonChangeLapBeepPath_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select WAV"), NULL, tr("sound files (*.wav)"));
    if(!fileName.isEmpty()){
        Settings::instance()->setLapBeepPath(fileName);
        this->ui->labelSettingsLapBeep->setText(fileName);
    }
}

void MainWindow::on_buttonChangeFastestLapShout_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select WAV"), NULL, tr("sound files (*.wav)"));
    if(!fileName.isEmpty()){
        Settings::instance()->setFastestLapSoundPath(fileName);
        this->ui->labelSettingsFastestLapShoutPath->setText(fileName);
    }
}

void MainWindow::onStopRaceClicked()
{
    CurrentRace::instance()->stopRace();
    m_pRaceTableWidget->updateTable();
    this->ui->tabWidget->setCurrentIndex(2);
}
