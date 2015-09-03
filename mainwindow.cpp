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

    // pilots table
    m_pPilotsModel = new QSqlTableModel(this, this->m_pRLTDatabase->database());
    m_pPilotsModel->setTable("pilots");
    m_pPilotsModel->setEditStrategy(QSqlTableModel::OnRowChange);
    m_pPilotsModel->select();
    m_pPilotsModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_pPilotsModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_pPilotsModel->setHeaderData(2, Qt::Horizontal, tr("API-Token"));
    m_pPilotsModel->setHeaderData(3, Qt::Horizontal, tr("Quad-Token"));

    this->ui->m_uiTableViewPilots->setModel(m_pPilotsModel);
    this->ui->m_uiTableViewPilots->hideColumn(0); // don't show the ID

    // race table
    m_pRacesModel =  new QSqlTableModel(this, this->m_pRLTDatabase->database());
    m_pRacesModel->setTable("races");
    m_pRacesModel->setEditStrategy(QSqlTableModel::OnRowChange);
    m_pRacesModel->select();
    m_pRacesModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_pRacesModel->setHeaderData(1, Qt::Horizontal, tr("Title"));
    m_pRacesModel->setHeaderData(2, Qt::Horizontal, tr("Started"));

    this->ui->m_tableViewRaces->setModel(m_pRacesModel);
    this->ui->m_tableViewRaces->hideColumn(0); // don't show the ID

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSimulatorLog_clicked()
{
    CurrentRace::instance()->incommingPilotSignal(this->ui->lineEdit_SimulatorToken->text());
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
    qDebug() << "bytes:" << bytes;
    CurrentRace::instance()->incommingPilotSignal(QString(bytes).replace("\r",""));
}

void MainWindow::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}

void MainWindow::on_buttonStartRace_clicked()
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
    this->ui->labelCurrentRaceTitle->setText(v);
}

void MainWindow::onCurrentRacePilotDataChanged(){

    this->ui->tableWidget->clearContents();
    this->ui->tableWidget->setColumnCount(5);
    this->ui->tableWidget->setRowCount(CurrentRace::instance()->getPilotsList()->size());
    this->ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Pilot"));
    this->ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Laps"));
    this->ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Last Lap Time"));
    this->ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("Fasted Lap"));
    this->ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Fasted Lap Time"));

    for (int i = 0; i < CurrentRace::instance()->getPilotsList()->size(); ++i) {

        RacePilot *pilot = CurrentRace::instance()->getPilotsList()->at(i);

        // pilot name
        QTableWidgetItem *itemPilotName = new QTableWidgetItem(pilot->getPilotName());
        this->ui->tableWidget->setItem(i,0,itemPilotName);

        // lap count
        QTableWidgetItem *itemLapCount = new QTableWidgetItem(QString("%1").arg(pilot->lapCount()-1));
        this->ui->tableWidget->setItem(i,1,itemLapCount);

        // last lap time
        if(pilot->hasLastLapTime()){
            QTableWidgetItem *itemLastLapTime = new QTableWidgetItem(QString("%1").arg(pilot->lastLapTimeString()));
            this->ui->tableWidget->setItem(i,2,itemLastLapTime);
        }

        // fastest lap // fastest lap time
        if(pilot->getFastedLap()){
            QTableWidgetItem *itemFastestLap = new QTableWidgetItem(QString("%1").arg(pilot->getFastedLap()->getLap()));
            this->ui->tableWidget->setItem(i,3,itemFastestLap);

            QTableWidgetItem *itemFastestLapTime = new QTableWidgetItem(QString("%1").arg(pilot->getFastedLap()->formatedLapTime()));
            this->ui->tableWidget->setItem(i,4,itemFastestLapTime);
        }

    }
}

void MainWindow::onCurentRaceFinished(){
    this->ui->labelFastestLapData->setText("");
    this->ui->tableWidget->clearContents();
    this->ui->labelCurrentRaceTitle->setText("");
}

void MainWindow::onCurrentRaceFastestLapDataChanged(){
    // updating fastest pilot
    RacePilot *fastestPilot = CurrentRace::instance()->getFastestPilot();
    if(fastestPilot){
       this->ui->labelFastestLapData->setText(QString("Pilot: %1 Lap: %2 Time: %3").arg(fastestPilot->getPilotName()).arg(fastestPilot->getFastedLap()->getLap()).arg(fastestPilot->getFastedLap()->formatedLapTime()));
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

void MainWindow::on_buttonStopRace_clicked()
{
    CurrentRace::instance()->stopRace();
}
