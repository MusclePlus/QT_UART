#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    QStringList serialNamePort;

    serialPort = new QSerialPort(this);

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));//手动创建信号槽，触发源为接收到数据

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        serialNamePort << info.portName();
    }
    ui->uart_Box->addItems(serialNamePort);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

void MyMainWindow::serialPortReadyRead_Slot()
{
    QByteArray buffer = serialPort->readAll();
    // 将QByteArray数据类型转换，要能正确显示中文，需要使用QString::fromLocal8Bit
    ui->recvEdit->appendPlainText(QString::fromLocal8Bit(buffer));
    qDebug("接收数据");
}

void MyMainWindow::on_OpenUart_Btn_clicked()
{
//    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBist;
    QSerialPort::Parity checkBist;

//    if(ui->Baud_Box->currentText() == "4800"){
//        baudRate = QSerialPort::Baud4800;
//    }else if(ui->Baud_Box->currentText() == "9600"){
//        baudRate = QSerialPort::Baud9600;
//    }else if(ui->Baud_Box->currentText() == "115200"){
//        baudRate = QSerialPort::Baud115200;
//    }




    if(ui->Data_Box->currentText() == "5"){
        dataBits = QSerialPort::Data5;
    }else if(ui->Data_Box->currentText() == "6"){
        dataBits = QSerialPort::Data6;
    }else if(ui->Data_Box->currentText() == "7"){
        dataBits = QSerialPort::Data7;
    }else if(ui->Data_Box->currentText() == "8"){
        dataBits = QSerialPort::Data8;
    }

    if(ui->Stop_Box->currentText() == "1"){
        stopBist = QSerialPort::OneStop;
    }else if(ui->Stop_Box->currentText() == "1.5"){
        stopBist = QSerialPort::OneAndHalfStop;
    }else if(ui->Stop_Box->currentText() == "2"){
        stopBist = QSerialPort::TwoStop;
    }

    if(ui->Cheak_Box->currentText() == "none"){
        checkBist = QSerialPort::NoParity;
    }

    serialPort->setPortName(ui->uart_Box->currentText());
//    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBist);
    serialPort->setParity(checkBist);

    if(serialPort->open(QIODevice::ReadWrite) == true){
        QMessageBox::information(this,"提示","成功");
    }else{
        QMessageBox::critical(this,"提示","失败");
    }
    qDebug("打开串口");


}

//编写一个函数 使得 qint32 转化为 QString ，大概思路为 创建一个临时变量 temp，对 10 求余获取到个位，再加上 '0' 的 ascall码，最后使 a/10 ，进行倒数第二个数值的获取
//最后由于上面是先从个位取值，所以反向 赋值即可
void MyMainWindow::int2Chars(char str[],long long int a,int &length){
    length=0;
    char temp[20];
    while(a){
        temp[length]='0'+a%10;
        a=a/10;
        length++;
    }
    str[length]=0;
    int i,j;
    i=0;j=length-1;
    for(;i<length;i++,j--){
        str[i]=temp[j];
    }
    return;
}


void MyMainWindow::on_CheckUart_Btn_clicked()
{
    ui->uart_Box->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //把后面每个项 依次赋值给 info
    {
        ui->uart_Box->addItem(info.portName());
     }
      QList<int> baudList=QSerialPortInfo::standardBaudRates();
      for(int i=0;i<baudList.length();i++){
          char str[20];
          int length;
          int2Chars(str,baudList.at(i),length);
          ui->Baud_Box->addItem(QString(QByteArray(str,length )));
      }
      ui->OpenUart_Btn->setEnabled(true);//使能打开串口

}

void MyMainWindow::on_SendData_Btn_clicked()
{
    serialPort->write(ui->sendEdit->text().toLocal8Bit().data());
    qDebug("发送数据");
}

void MyMainWindow::on_CloseUart_Btn_clicked()
{
    serialPort->close();
    qDebug("关闭串口");
}

void MyMainWindow::on_ClearData_Btn_clicked()
{
    ui->sendEdit->clear();
    ui->recvEdit->clear();
    qDebug("清除数据");
}
