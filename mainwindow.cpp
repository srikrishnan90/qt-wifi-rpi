#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList list2;
    QProcess process1;
    process1.start("sh",QStringList()<<"-c"<<"sudo iwlist wlan0 scan | grep ESSID");//scan list of wifi networks
    process1.waitForFinished();
    QString data = process1.readAllStandardOutput();
    QString Error= process1.readAllStandardError();
    list2 = (QStringList()<<"------Select-------");//append to dropdownlist
    ui->comboBox->addItems(list2);
    QStringList list = data.split("\n");//split data
    for(int i=0;i<list.count()-1;i++)
    {
        QStringList list1 = list.at(i).split("ESSID:");
        QString data1 = list1.at(1);
        list2 = (QStringList()<<data1);
        ui->comboBox->addItems(list2);//adding wifi names to dropdownlist
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int n=0;
    QString line;
    QString lines[1000];
    QString pwd=ui->lineEdit->text();

//    QFile file1("/etc/wpa_supplicant/wpa_supplicant.conf");
//    if(file1.open(QIODevice::ReadOnly|QIODevice::Text))
//    {
//        QTextStream stream(&file1);
//        do {
//            line = stream.readLine();
//            lines[n]=line;
//            qDebug() << lines[n];
//            n++;
//        } while(!line.isNull());
//        file1.close();
//        qDebug() << "Reading finished";
//    }

    QFile file("/etc/wpa_supplicant/wpa_supplicant.conf");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
//        for(int i=0;i<=n;i++)
//        {
//            stream << lines[i]+"\n";
//        }
        stream<<"ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n";
        stream<<"update_config=1\n";
        stream<<"country=IN\n";
        stream<<"\n";
        stream<<"network={\n";
        stream<<"\tssid=";
        stream<<ui->comboBox->currentText()+"\n";
        stream<<"\tpsk=\"";
        stream<<ui->lineEdit->text()+"\"\n";
        stream<<"\tkey_mgmt=WPA-PSK\n";
        stream<<"}";

        file.close();
        QProcess process2;
        process2.start("sh",QStringList()<<"-c"<<"sudo wpa_cli -i wlan0 reconfigure");
        process2.waitForFinished();
    }

}
