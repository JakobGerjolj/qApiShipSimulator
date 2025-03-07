#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CanHandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_11_clicked();

    void on_speedSlider_16_sliderMoved(int position);

    void on_pushButton_12_clicked();

    void on_speedSlider_17_sliderMoved(int position);

    void on_pushButton_13_clicked();

    void on_speedSlider_18_sliderMoved(int position);

    void on_pushButton_14_clicked();

    void on_speedSlider_19_sliderMoved(int position);

    void on_pushButton_15_clicked();

    void on_speedSlider_20_sliderMoved(int position);

    void on_pushButton_16_clicked();

    void on_speedSlider_21_sliderMoved(int position);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_17_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_5_clicked();

    void on_speedSlider_22_sliderMoved(int position);

    void on_pushButton_18_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_8_clicked();

    void on_radioButton_9_clicked();

    void on_radioButton_13_clicked();

    void on_pushButton_19_clicked();

    void on_radioButton_14_clicked();

    void on_radioButton_15_clicked();

    void on_radioButton_16_clicked();

    void on_radioButton_17_clicked();

    void on_pushButton_20_clicked();

    void on_radioButton_18_clicked();

    void on_radioButton_19_clicked();

    void on_radioButton_20_clicked();

    void on_radioButton_21_clicked();

    void on_pushButton_21_clicked();

    void on_radioButton_26_clicked();

    void on_radioButton_27_clicked();

    void on_radioButton_28_clicked();

    void on_radioButton_29_clicked();

    void on_pushButton_22_clicked();

    void on_speedSlider_23_sliderMoved(int position);

    void on_pushButton_23_clicked();

    void on_speedSlider_24_sliderMoved(int position);

    void on_pushButton_24_clicked();

    void on_radioButton_30_clicked();

    void on_radioButton_31_clicked();

    void on_radioButton_32_clicked();

    void on_radioButton_33_clicked();

    void on_pushButton_25_clicked();

    void on_textEdit_textChanged();

    void on_pushButton_26_clicked();

    void on_textEdit_2_textChanged();

    void on_pushButton_27_clicked();

    void on_textEdit_3_textChanged();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

private:
    Ui::MainWindow *ui;

    bool areWeSendingHiCellVoltage{false};
    bool areWeSendingLoCellVoltage{false};
    bool areWeSendingChargerTemp{false};
    bool areWeSendingChargerVoltage{false};
    bool areWeSendingChargerCurrent{false};
    bool areWeSendingInVoltage{false};
    bool areWeSendingInCurrent{false};
    bool areWeSendingPortMotorStatus{false};
    bool areWeSendingBatteryStatus{false};
    bool areWeSendingChargerStatus{false};
    bool areWeSendingStbMotorStatus{false};
    bool areWeSendingDCDCVoltage{false};
    bool areWeSendingDCDCCurrent{false};
    bool areWeSendingDCDCStatus{false};
    bool areWeSendingMaxPortPower{false};
    bool areWeSendingFullChargeBattery{false};
    bool areWeSendingShoreLimit{false};

    CanHandler* m_CanHandler;
};
#endif // MAINWINDOW_H
