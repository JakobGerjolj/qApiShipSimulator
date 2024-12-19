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

