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
    void on_c_btn_connect_clicked();

    void on_m_btn_move_clicked();

    void on_c_btn_jogLeft_clicked();

    void on_c_btn_jogRight_clicked();

    void on_pushButton_clicked();

    void on_c_btn_fresh_clicked();

    void on_c_spinbox_setJogStep_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    int m_handle;
    void init_mover();
};
#endif // MAINWINDOW_H
