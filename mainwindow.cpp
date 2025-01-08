#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_CanHandler = new CanHandler;

    ui -> PortBatInfoLabel_2 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_2 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_3 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_3 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_4 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_4 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_5 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_5 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_6 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_6 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_7 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_7 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_7 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_7 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_8 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_8 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_9 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_9 -> setStyleSheet("QLabel { background-color : red; }");

    ui -> PortBatInfoLabel_10 -> setText("Not sending messages");
    ui -> PortBatInfoLabel_10 -> setStyleSheet("QLabel { background-color : red; }");

}

MainWindow::~MainWindow()
{
    delete m_CanHandler;
    delete ui;
}



void MainWindow::on_pushButton_11_clicked()
{

    m_CanHandler -> toggleHiCellVoltageSend();

    if(!areWeSendingHiCellVoltage){
        areWeSendingHiCellVoltage = true;
        ui -> PortBatInfoLabel_2 -> setText("Sending messages");
        ui -> PortBatInfoLabel_2 -> setStyleSheet("QLabel { background-color : green; }");
    }else{
        areWeSendingHiCellVoltage = false;
        ui -> PortBatInfoLabel_2 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_2 -> setStyleSheet("QLabel { background-color : red; }");

    }
}


void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_speedSlider_16_sliderMoved(int position)
{

    m_CanHandler -> setHighCellVoltage(QString::number(position));

}


void MainWindow::on_pushButton_12_clicked()
{

    m_CanHandler -> toggleLoCellVoltageSend();

    if(!areWeSendingLoCellVoltage){
        areWeSendingLoCellVoltage = true;
        ui -> PortBatInfoLabel_3 -> setText("Sending messages");
        ui -> PortBatInfoLabel_3 -> setStyleSheet("QLabel { background-color : green; }");
    }else{
        areWeSendingLoCellVoltage = false;
        ui -> PortBatInfoLabel_3 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_3 -> setStyleSheet("QLabel { background-color : red; }");

    }

}


void MainWindow::on_speedSlider_17_sliderMoved(int position)
{

    m_CanHandler -> setLowCellVoltage(QString::number(position));

}


void MainWindow::on_pushButton_13_clicked()
{

    m_CanHandler -> toggleChargerTempSend();

    if(!areWeSendingChargerTemp){
        areWeSendingChargerTemp = true;
        ui -> PortBatInfoLabel_4 -> setText("Sending messages");
        ui -> PortBatInfoLabel_4 -> setStyleSheet("QLabel { background-color : green; }");
    }else{
        areWeSendingChargerTemp = false;
        ui -> PortBatInfoLabel_4 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_4 -> setStyleSheet("QLabel { background-color : red; }");

    }

}


void MainWindow::on_speedSlider_18_sliderMoved(int position)
{

    m_CanHandler -> setChargerTemp(QString::number(position));

}


void MainWindow::on_pushButton_14_clicked()
{

    m_CanHandler -> toggleChargerVoltageSend();

    if(!areWeSendingChargerVoltage){
        areWeSendingChargerVoltage = true;
        ui -> PortBatInfoLabel_5 -> setText("Sending messages");
        ui -> PortBatInfoLabel_5 -> setStyleSheet("QLabel { background-color : green; }");

    }else{
        areWeSendingChargerVoltage = false;
        ui -> PortBatInfoLabel_5 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_5 -> setStyleSheet("QLabel { background-color : red; }");

    }

}


void MainWindow::on_speedSlider_19_sliderMoved(int position)
{

    m_CanHandler -> setChargerVoltage(QString::number(position));

}


void MainWindow::on_pushButton_15_clicked()
{

    m_CanHandler -> toggleChargerCurrentSend();

    if(!areWeSendingChargerCurrent){
        areWeSendingChargerCurrent = true;
        ui -> PortBatInfoLabel_6 -> setText("Sending messages");
        ui -> PortBatInfoLabel_6 -> setStyleSheet("QLabel { background-color : green; }");

    }else{
        areWeSendingChargerCurrent = false;
        ui -> PortBatInfoLabel_6 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_6 -> setStyleSheet("QLabel { background-color : red; }");

    }

}


void MainWindow::on_speedSlider_20_sliderMoved(int position)
{

    m_CanHandler -> setChargerCurrent(QString::number(position));

}


void MainWindow::on_pushButton_16_clicked()
{
    m_CanHandler -> toggleChargerInVoltageSend();

    if(!areWeSendingInVoltage){
        areWeSendingInVoltage = true;
        ui -> PortBatInfoLabel_7 -> setText("Sending messages");
        ui -> PortBatInfoLabel_7 -> setStyleSheet("QLabel { background-color : green; }");

    }else{
        areWeSendingInVoltage = false;
        ui -> PortBatInfoLabel_7 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_7 -> setStyleSheet("QLabel { background-color : red; }");

    }
}


void MainWindow::on_speedSlider_21_sliderMoved(int position)
{

    m_CanHandler -> setChargerInVoltage(QString::number(position));

}


void MainWindow::on_radioButton_clicked()
{

    m_CanHandler -> setChargerInInstance("0");

}


void MainWindow::on_radioButton_2_clicked()
{

    m_CanHandler -> setChargerInInstance("1");

}


void MainWindow::on_radioButton_3_clicked()
{

    m_CanHandler -> setChargerInInstance("2");

}


void MainWindow::on_pushButton_17_clicked()
{

    m_CanHandler -> toggleChargerInCurrent();

    if(!areWeSendingInVoltage){
        areWeSendingInVoltage = true;
        ui -> PortBatInfoLabel_8 -> setText("Sending messages");
        ui -> PortBatInfoLabel_8 -> setStyleSheet("QLabel { background-color : green; }");

    }else{
        areWeSendingInVoltage = false;
        ui -> PortBatInfoLabel_8 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_8 -> setStyleSheet("QLabel { background-color : red; }");

    }

}


void MainWindow::on_radioButton_4_clicked()
{

    m_CanHandler -> setChargerInCurrentInstance("0");

}

void MainWindow::on_radioButton_6_clicked()
{

    m_CanHandler -> setChargerInCurrentInstance("1");

}

void MainWindow::on_radioButton_5_clicked()
{

    m_CanHandler -> setChargerInCurrentInstance("2");

}


void MainWindow::on_speedSlider_22_sliderMoved(int position)
{

    m_CanHandler -> setChargerInCurrent(QString::number(position));

}


void MainWindow::on_pushButton_18_clicked()
{

    m_CanHandler -> togglePortMotor();

    if(!areWeSendingPortMotorStatus){
        areWeSendingPortMotorStatus = true;
        ui -> PortBatInfoLabel_9 -> setText("Sending messages");
        ui -> PortBatInfoLabel_9 -> setStyleSheet("QLabel { background-color : green; }");

    }else{
        areWeSendingPortMotorStatus = false;
        ui -> PortBatInfoLabel_9 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_9 -> setStyleSheet("QLabel { background-color : red; }");

    }

}

void MainWindow::on_radioButton_7_clicked()
{

    m_CanHandler -> setPortMotorStatus("0");

}

void MainWindow::on_radioButton_8_clicked()
{

    m_CanHandler -> setPortMotorStatus("1");

}

void MainWindow::on_radioButton_9_clicked()
{

    m_CanHandler -> setPortMotorStatus("2");

}


void MainWindow::on_radioButton_13_clicked()
{

    m_CanHandler -> setPortMotorStatus("3");

}


void MainWindow::on_pushButton_19_clicked()
{


    m_CanHandler -> toggleBatteryS();

    if(!areWeSendingBatteryStatus){
        areWeSendingBatteryStatus = true;
        ui -> PortBatInfoLabel_10 -> setText("Sending messages");
        ui -> PortBatInfoLabel_10 -> setStyleSheet("QLabel { background-color : green; }");

    }else{
        areWeSendingBatteryStatus = false;
        ui -> PortBatInfoLabel_10 -> setText("Not sending messages");
        ui -> PortBatInfoLabel_10 -> setStyleSheet("QLabel { background-color : red; }");

    }

}


void MainWindow::on_radioButton_14_clicked()
{

    m_CanHandler -> setBatteryStatus("0");

}


void MainWindow::on_radioButton_15_clicked()
{

    m_CanHandler -> setBatteryStatus("1");

}


void MainWindow::on_radioButton_16_clicked()
{

    m_CanHandler -> setBatteryStatus("2");

}


void MainWindow::on_radioButton_17_clicked()
{

    m_CanHandler -> setBatteryStatus("3");

}

