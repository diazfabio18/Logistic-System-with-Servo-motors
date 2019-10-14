#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mach.h"
//#include "SerialPortWriter.h"
//#include "SeriaPortReader.h"
//#include "settingsdialog.h"

#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    initActionsConnections();

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::exit_clicked()
{
    closeSerialPort();
    QApplication::quit();
}

void MainWindow::conect_clicked(QByteArray data){
    int static flag = 0;

    MainWindow::setVisible(false);
    qErrnoWarning(data);
    qErrnoWarning("%s",QString(data));
    if(QString (data) == "K"){//Verifico que me conterte lo que pido
        qErrnoWarning("entre con data igual a (%s)",QString (data));
        closeSerialPort();
        disconnect(&m_timer, &QTimer::timeout, this, &MainWindow::timeout);

        Mach m;
        if(m.exec()==QDialog::Accepted){    //Abro la ventana del dispositivo

        }
    }
    else if(flag == 0){
        QMessageBox::critical(this, tr("Error al verificar el dato, reintentando"), m_serial->errorString());   //En caso de no poder conectarse
        writeData("K");
        connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
        flag = 1;
    }
    else QMessageBox::critical(this, tr("Error al verificar el dato, reinicie el dispositivo"), m_serial->errorString());

}


void MainWindow::openSerialPort(){

    m_serial->setPortName("COM3");
    m_serial->setBaudRate(9600);
    m_serial->setDataBits(QSerialPort::DataBits(8));
    m_serial->setParity(QSerialPort::Parity(0));
    m_serial->setStopBits(QSerialPort::StopBits(1));
    m_serial->setFlowControl(QSerialPort::FlowControl(0));
    if (m_serial->open(QIODevice::ReadWrite)) {
       qErrnoWarning("Puerto conectado");
       connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
       writeData("K");
       connect(&m_timer, &QTimer::timeout, this, &MainWindow::timeout);
      if (!m_timer.isActive())  m_timer.start(5000);

    } else {
        QMessageBox::critical(this, tr("Error al conectarse al puerto"), m_serial->errorString());
    }
}

void MainWindow::timeout(){
     disconnect(m_serial,&QSerialPort::readyRead,this,&MainWindow::readData);
     closeSerialPort();
     QMessageBox::critical(this, tr("Error, no hubo respuesta"), m_serial->errorString());
     disconnect(&m_timer, &QTimer::timeout, this, &MainWindow::timeout);
}



void MainWindow::closeSerialPort(){

    if (m_serial->isOpen()){
        m_serial->close();
       qErrnoWarning("Puerto desconectado");
    }
    else{
        qErrnoWarning("El puerto no estaba conectado");
    }

}

void MainWindow::initActionsConnections(){

    connect(ui->BT_conect, &QPushButton::clicked, this, &MainWindow::openSerialPort);
    //connect(ui->BT_conect, &QPushButton::clicked, this, &MainWindow::inicia);
    connect(ui->BT_exit, &QPushButton::clicked, this, &MainWindow::exit_clicked);

}

void MainWindow::inicia(){
    Mach m;
    if(m.exec()==QDialog::Accepted){

    }
}


void MainWindow::writeData(const QByteArray &data){

    m_serial->write(data);
}

void MainWindow::readData(){

    const QByteArray data = m_serial->read(1);//m_serial->readAll();
    conect_clicked(data);

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}
