#ifndef MACH_H
#define MACH_H

#include "mainwindow.h"
#include <QDialog>

namespace Ui {
class Mach;
}

class Mach : public QDialog
{
    Q_OBJECT

public:

    explicit Mach(QWidget *parent = nullptr);
    ~Mach();

    int exec();




private slots:

    void recibir_color();
    void guardar_RGB();
    void guardar_salida();
    void refresh_LCD();


    int initActionsConnections();
    int Re_iniciar();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void Mandar_encendido();
    void Mandar_detener();
    void Mandar_salida();
    void closeSerialPort();
    void cerrar();


    void on_BT_send_to_inventory_clicked();
    void on_BT_out_clicked();





private:
    QSerialPort *m_serial = nullptr;
    int Encendido = 0;
    QTimer m_timer;
    int RGB[3];
    int inRGB[3];
    Ui::Mach *ui;

};

#endif // MACH_H
