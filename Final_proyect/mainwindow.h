#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-------------------Includes----------------------
#include <QCoreApplication>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QByteArray>
#include <QTextStream>
#include <QTimer>
#include <QDialog>
#include <QObject>
//#include <QtSerialPort/QtSerialPort>
#include <QSerialPort>
#include <QLabel>
#include <QMessageBox>


//----------------Constands-----------------------
#define OK 1
#define ERROR -1


//---------------MainWidnow-----------------------
namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //************************************************************

private slots:

//----------In functions---------------------------

    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void conect_clicked(QByteArray data);
    void timeout();
    void exit_clicked();


private:
    void initActionsConnections();

//**********Funcion de prueba******************
    void inicia();


private:
 //----------Conection Window----------------------
    Ui::MainWindow *ui;
    QLabel *m_status = nullptr;
   //SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
    QTimer m_timer;
};

#endif // MAINWINDOW_H
