#include "ui_mach.h"
#include "mainwindow.h"
#include "mach.h"
//#include "SerialPortWriter.h"
//#include "SeriaPortReader.h"
#include <thread>
#include <iostream>
#include <QThread>
#include <fstream>
#include <iostream>
#include <time.h>
#include <QMessageBox>

#include <QLabel>

//-------------------Global vars------------------------


Mach::Mach(QWidget *parent) :
    QDialog(parent),
    m_serial(new QSerialPort(this)),
    ui(new Ui::Mach)
{
    ui->setupUi(this);
    if(initActionsConnections() == OK){    //Si falla al habrir el puerto, no se inicia
      FILE *A;
      A = fopen("../Data_RGB.txt", "r");  //Directorio donde va a estar el archivo
      if(!A){
        qErrnoWarning("Error al leer el archivo");
        QCoreApplication::exit();
      }
      fscanf(A ,"%d %d %d %d %d %d", &inRGB[0], &inRGB[1], &inRGB[2], &RGB[0], &RGB[1],&RGB[2]);   //Cargalos valores del archivo
      refresh_LCD();
      fclose(A);
    }else {
       qErrnoWarning("Error en el puerto, finalizando programa");
       QApplication::exit(-1);
    }

}

Mach::~Mach(){

    guardar_RGB();
    delete ui;
}

int Mach::exec(){

    return QDialog::exec();
}


//**********************Funciones de los LCD y archivos***********************************************
void Mach::on_BT_send_to_inventory_clicked(){

    inRGB[0] += RGB[0];  //Reseteo los valores del vec RGB y los mando al inventario
    inRGB[1] += RGB[1];
    inRGB[2] += RGB[2];
    RGB[0] = 0;
    RGB[1] = 0;
    RGB[2] = 0;

    refresh_LCD(); //Cambio los valores de los displays
}

void Mach::on_BT_out_clicked(){//Vacio el inventario y lo envio al archivo

    guardar_salida();
    inRGB[0] = 0;
    inRGB[1] = 0;
    inRGB[2] = 0;
    refresh_LCD();
}

void Mach::guardar_RGB(){ //Guarda en un archivo los valores de RGB del inventario

    FILE *A;
    A = fopen("../Data_RGB.txt", "w");
    if(!A){
        qErrnoWarning("Error al escribir el archivo");
        return;
    }

    else{
        rewind(A);
        fprintf(A, "%d %d %d %d %d %d", inRGB[0], inRGB[1], inRGB[2], RGB[0], RGB[1], RGB[2]);
        fclose(A);
    }
}

void Mach::guardar_salida(){

    int ch;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    FILE *A;
    A = fopen("/home/sebas/Final_proyect/Informacion de salida", "r+");
    if(!A){
        qErrnoWarning("Error al escribir el archivo de salida");
        return;
    }

    else{
        while(ch!=EOF){
                ch = fgetc(A);
        }
        fprintf(A,"Salida de producto con fecha %d/%d/%d a las %d:%d:%d con: \n-Rojo: %d\n-Azul: %d\n-Verde: %d \n\n",
                tm.tm_mday, tm.tm_mon, tm.tm_year +1900, tm.tm_hour, tm.tm_min, tm.tm_sec, inRGB[0], inRGB[2], inRGB[1]);
        fclose(A);

    }

}

void Mach::refresh_LCD(){

//-------------Refresca los valores del display---------------
    ui->LCD_GI_blu->display(inRGB[2]);
    ui->LCD_GI_gren->display(inRGB[1]);
    ui->LCD_GI_red->display(inRGB[0]);
    ui->LCD_MI_blu->display(RGB[2]);
    ui->LCD_MI_gren->display(RGB[1]);
    ui->LCD_MI_red->display(RGB[0]);
}

//***************************Funciones del serialport*************************************************


int Mach::initActionsConnections(){
    if(Re_iniciar() == OK){
        connect(ui->BT_Start, &QPushButton::clicked, this, &Mach::Mandar_encendido);
        connect(ui->BT_Stop, &QPushButton::clicked, this, &Mach::Mandar_detener);
        connect(ui->BT_Exit, &QPushButton::clicked, this, &Mach::Mandar_salida);
        return OK;
    }
    return ERROR;
}

int Mach::Re_iniciar(){

    m_serial->setPortName("COM3");
    m_serial->setBaudRate(9600);
    m_serial->setDataBits(QSerialPort::DataBits(8));
    m_serial->setParity(QSerialPort::Parity());
    m_serial->setStopBits(QSerialPort::StopBits(1));
    m_serial->setFlowControl(QSerialPort::FlowControl());
    if (m_serial->open(QIODevice::ReadWrite)) return OK;
    return ERROR;
}

void Mach::writeData(const QByteArray &data){

    m_serial->write(data);
}

void Mach::readData(){

   // const QByteArray data = m_serial->readAll();
    recibir_color();

}

void Mach::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());

        //closeSerialPort();
    }
}

void Mach::Mandar_encendido(){

    if(Encendido == 0) {
        connect(m_serial,&QSerialPort::readyRead,this,&Mach::recibir_color);
        writeData("E");
        Encendido = 1;
    }
    else{
        qWarning("El dispositivo se encuentra encendido");
    }

}

void Mach::Mandar_detener(){

    if(Encendido == 1) {
        writeData("D");
       // disconnect(m_serial,&QSerialPort::readyRead,this,&Mach::recibir_color);
        Encendido = 0;
    }
    else{
        qWarning("El dispositivo se encuentra detenido");
    }

}

void Mach::Mandar_salida(){
   // Mandar_detener();
    writeData("S");
    disconnect(m_serial,&QSerialPort::readyRead,this,&Mach::recibir_color);
    guardar_RGB();
    m_timer.start(1000);
    connect(&m_timer, &QTimer::timeout, this, &Mach::cerrar);
}
void Mach::cerrar(){
    closeSerialPort();
    QApplication::exit(0);
}

void Mach::closeSerialPort(){

    if (m_serial->isOpen()){
        m_serial->close();
       qErrnoWarning("Puerto desconectado");
    }
    else{
        qErrnoWarning("El puerto no estaba conectado");
    }

}




void Mach::recibir_color(){

    const QByteArray data = m_serial->readAll();
    qErrnoWarning(data);
    if(data == "R") RGB[0]++;
    else if(data == "G") RGB[1]++;
    else if(data == "B") RGB[2]++;

    else qErrnoWarning("Caracter desconocido");

    refresh_LCD();
}
