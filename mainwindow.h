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

private:
    Ui::MainWindow *ui;

    bool areWeSendingHiCellVoltage{false};
    bool areWeSendingLoCellVoltage{false};
    bool areWeSendingChargerTemp{false};
    bool areWeSendingChargerVoltage{false};
    bool areWeSendingChargerCurrent{false};
    bool areWeSendingInVoltage{false};
    bool areWeSendingInCurrent{false};

    CanHandler* m_CanHandler;
};
#endif // MAINWINDOW_H
