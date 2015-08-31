#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "currentrace.h"
#include "qextserialport/qextserialenumerator.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSimulatorLog_clicked()
{
    CurrentRace::instance()->incommingPilotSignal(this->ui->lineEdit_SimulatorToken->text());
    CurrentRace::instance()->updateTableWidget(this->ui->tableWidget,this->ui->labelFastestLapData);
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
    CurrentRace::instance()->updateTableWidget(this->ui->tableWidget,this->ui->labelFastestLapData);
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
