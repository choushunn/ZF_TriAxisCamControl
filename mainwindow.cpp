#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_mover();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
初始化位移台
*/
void MainWindow::init_mover(){
    // 获取串口列表
    char serialList[1024] = {0};
    int ret = ::listPorts(serialList, 1024);
    if (ret < 0)
    {
        qDebug() << "获取串口列表失败!";
        // ui->label_msg->setText("获取串口列表失败!");
    }
    // 获取系统串口
    QStringList SerialPortList = QString(serialList).split(',');
    ui->c_Combobox_SP->addItems(SerialPortList);

    // 获取位移台型号名称列表
    char modelsList[1024] = {0};
    ret = ::getAllModels(modelsList, 1024);
    if (ret < 0)
    {
        qDebug() << ("获取位移台型号名称列表失败!");
    }
    // ui->c_PlainText_msg->ensureCursorVisible(); // 确保光标可见并滚动到底部
}

/*
连接位移台
*/
void MainWindow::on_c_btn_connect_clicked()
{
    QByteArray nameArray = ui->c_Combobox_SP->currentText().toLocal8Bit();
    char *name = nameArray.data();

    m_handle = ::openEmcvx(name);            // 打开控制器设备
    if (m_handle <= 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("打开串口设备失败! ret: %1").arg(m_handle));
        ui->c_btn_connect->setText("打开");
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("打开串口设备成功"));
        ui->c_btn_connect->setText("断开");
    }

    // 初始化轴设备
    int ID = ui->c_ComboBox_axisNumber->currentIndex() + 1;
    QByteArray modelArray = ui->c_ComboBox_modelName->currentText().toLocal8Bit();
    char *model = modelArray.data();
    int axisCount = ::getDeviceCode(m_handle);

    int ret = ::initAxis(m_handle, ID, model, axisCount);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("初始化控制器设备失败! ret = %1").arg(ret));
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText("连接控制器设备成功");
    }
}

/*
移动到指定位置
*/
void MainWindow::on_m_btn_move_clicked()
{
    int ID = ui->c_ComboBox_axisNumber->currentIndex() + 1;
    float disp = ui->c_spinbox_setJogStep->text().toFloat();

    // 设置move的绝对位置
    int ret = ::setAbsoluteDisp(m_handle, ID, disp);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置绝对位置失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置绝对位置成功， disp: %1").arg(disp));
    }

    // move运行
    ret = ::moveEmcvx(m_handle, ID, MOVE_CODE_MOVE);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("move运行失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("move运行成功"));
    }
}

/*
向左步进
*/
void MainWindow::on_c_btn_jogLeft_clicked()
{
    int ID = ui->c_ComboBox_axisNumber->currentIndex() + 1;
    float step = ui->c_spinbox_jogStep->text().toFloat();

    // 设置JOG运行的步长
    int ret = ::setJogStep(m_handle, ID, step);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长成功， step: %1").arg(step));
    }

    // 设置JOG运行的次数
    int time = ui->c_spinbox_Times->text().toInt();
    ret = ::setJogTime(m_handle, ID, time);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数成功， time: %1").arg(time));
    }

    // 设置JOG运行时每一步的停留延时时间
    int delay = ui->c_spinbox_jogDelay->text().toInt();
    ret = ::setJogDelay(m_handle, ID, delay);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间成功， delay: %1").arg(delay));
    }

    // 向负方向jog运行
    ret = ::moveEmcvx(m_handle, ID, MOVE_CODE_JOG_L);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("jog运行失败! ret: %1").arg(ret));
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("jog运行成功"));
    }
}

/*
向右步进
*/
void MainWindow::on_c_btn_jogRight_clicked()
{
    int ID = ui->c_ComboBox_axisNumber->currentIndex() + 1;
    float step = ui->c_spinbox_jogStep->text().toFloat();

    // 设置JOG运行的步长
    int ret = ::setJogStep(m_handle, ID, step);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长成功， step: %1").arg(step));
    }

    // 设置JOG运行的次数
    int time = ui->c_spinbox_Times->text().toInt();
    ret = ::setJogTime(m_handle, ID, time);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数成功， time: %1").arg(time));
    }

    // 设置JOG运行时每一步的停留延时时间
    int delay = ui->c_spinbox_jogDelay->text().toInt();
    ret = ::setJogDelay(m_handle, ID, delay);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间失败! ret: %1").arg(ret));
        return;
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间成功， delay: %1").arg(delay));
    }

    // 向正方向jog运行
    ret = ::moveEmcvx(m_handle, ID, MOVE_CODE_JOG_R);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText(QString("jog运行失败! ret: %1").arg(ret));
    }
    else
    {
        ui->c_PlainText_msg->appendPlainText(QString("jog运行成功"));
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->c_PlainText_msg->clear();
}

/*
刷新串口和设备
*/
void MainWindow::on_c_btn_fresh_clicked()
{
    ui->c_Combobox_SP->clear();
    ui->c_ComboBox_modelName->clear();

    // 获取串口列表
    char serialList[1024] = {0};
    int ret = ::listPorts(serialList, 1024);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText("获取串口列表失败!");
    }

    QStringList qstrList = QString(serialList).split(',');
    ui->c_Combobox_SP->addItems(qstrList);

    // 获取位移台型号名称列表
    char modelsList[1024] = {0};
    ret = ::getAllModels(modelsList, 1024);
    if (ret < 0)
    {
        ui->c_PlainText_msg->appendPlainText("获取位移台型号名称列表失败!");
    }

    qstrList = QString(modelsList).split(',');
    ui->c_ComboBox_modelName->addItems(qstrList);
}


void MainWindow::on_c_spinbox_setJogStep_valueChanged(int arg1)
{

}

/*
获取设置的当前位置
GetCurrentPos
*/

