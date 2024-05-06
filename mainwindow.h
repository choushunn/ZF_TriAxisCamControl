#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MoverLibrary.h"

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

    void on_m_btn_move_clicked();

    void on_m_spin_abs_position_valueChanged(double arg1);

    void on_m_spinx_jogStep_valueChanged(int arg1);

    void on_m_spin_jogNumber_valueChanged(int arg1);

    void on_m_spin_jogDelayTimes_valueChanged(int arg1);

    void on_m_spin_abs_position_editingFinished();

    void on_m_btn_stopJog_clicked();

    void on_m_btn_absStart_clicked();

private:
    Ui::MainWindow *ui;
    int m_handle;
    // void init_mover();
    // 初始化位移台
    void initMover();
    // 初始化UVC相机
    void initUVC();
    void getMoverCurrentPos();
    int m_jogStep = 0;
    int m_jogNum = 0;
    int m_jogDelay = 0;


};
#endif // MAINWINDOW_H
