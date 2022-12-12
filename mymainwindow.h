#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QString>

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();
    QSerialPort *serialPort;

private slots:

    void serialPortReadyRead_Slot();

    void on_OpenUart_Btn_clicked();

    void on_CheckUart_Btn_clicked();

    void on_SendData_Btn_clicked();

    void on_CloseUart_Btn_clicked();

    void on_ClearData_Btn_clicked();
    void int2Chars(char str[],long long int a,int &length);

private:
    Ui::MyMainWindow *ui;
};

#endif // MYMAINWINDOW_H
