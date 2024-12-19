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

private:
    Ui::MainWindow *ui;

    bool areWeSendingHiCellVoltage{false};
    bool areWeSendingLoCellVoltage{false};

    CanHandler* m_CanHandler;
};
#endif // MAINWINDOW_H
