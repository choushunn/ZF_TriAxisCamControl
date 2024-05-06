#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MoverLibrary.h"
#include "opencv2/opencv.hpp"
#include "cusbcamera.h"
#include <QDir>
#include <QTimer>
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

    void on_m_btn_freshMover_clicked();

    void on_m_btn_connect_mover_clicked();

    void on_m_btn_rest_clicked();

    void on_m_spin_abs_position_valueChanged(double arg1);

    void on_m_spin_jogStep_valueChanged(int arg1);

    void on_m_spin_jogNum_valueChanged(int arg1);

    void on_m_spin_jogDelay_valueChanged(int arg1);



    void on_m_btn_absStart_clicked();

    void showCameraFrame();

    void on_m_btn_openCamera_clicked();

    void takePhoto();
    void on_m_btn_snap_clicked();

    void on_m_btn_stop_clicked();

    void on_m_btn_startJog_clicked();

    void on_m_btn_record_clicked();

    double getMoverCurrentPos();
private:
    Ui::MainWindow *ui;
    int m_handle;
    cv::VideoCapture m_cap;
    CUSBCamera *m_usbCap = nullptr;
    int m_camIndex= 0;
    cv::Mat frame;
    // void init_mover();
    // 初始化位移台
    void initMover();
    // 初始化UVC相机
    void initUVC();

    int m_jogStep = 0;
    int m_jogNum = 0;
    int m_jogDelay = 0;
    QTimer *m_readTimer = nullptr;
    QTimer* m_takeTimer = nullptr;
    QTimer* m_moverTimer = nullptr;

};
#endif // MAINWINDOW_H
