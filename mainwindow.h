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

private:
    Ui::MainWindow *ui;
    int m_handle;
    // void init_mover();
    // 初始化位移台
    void initMover();
    // 初始化UVC相机
    void initUVC();
    void getMoverCurrentPos();


};
#endif // MAINWINDOW_H
