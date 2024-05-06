#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化位移台
    this->initMover();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMover()
{
    // 1.清空UI
    ui->m_cbx_sirial_list->clear();
    ui->m_cbx_modelName->clear();
    // 获取串口列表
    char serialList[1024] = {0};
    int ret = ::listPorts(serialList, 1024);
    if (ret < 0)
    {
        qDebug() << "获取串口列表失败!";
        statusBar()->showMessage("获取串口列表失败!");
    }else{
        // 获取系统可用串口
        QStringList SerialPortList = QString(serialList).split(',');
        ui->m_cbx_sirial_list->addItems(SerialPortList);
    }
    // 获取位移台型号名称列表
    char modelsList[1024] = {0};
    ret = ::getAllModels(modelsList, 1024);
    if (ret < 0)
    {
        qDebug() << "获取位移台型号名称列表失败!";
        statusBar()->showMessage("获取位移台型号名称失败!");
    }else{
        // qDebug() << modelsList;
        QStringList qstrList = QString(modelsList).split(',');
        ui->m_cbx_modelName->addItems(qstrList);
    }
}






// /*
// 连接位移台
// */
// void MainWindow::on_c_btn_connect_clicked()
// {
//     QByteArray nameArray = ui->m_cbx_sirial_list->currentText().toLocal8Bit();
//     char *name = nameArray.data();

//     m_handle = ::openEmcvx(name);            // 打开控制器设备
//     if (m_handle <= 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("打开串口设备失败! ret: %1").arg(m_handle));
//         ui->m_btn_connect_mover->setText("打开");
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("打开串口设备成功"));
//         ui->m_btn_connect_mover->setText("断开");
//     }

//     // 初始化轴设备
//     int ID = ui->m_cbx_axis->currentIndex() + 1;
//     QByteArray modelArray = ui->m_cbx_modelName->currentText().toLocal8Bit();
//     char *model = modelArray.data();
//     int axisCount = ::getDeviceCode(m_handle);

//     int ret = ::initAxis(m_handle, ID, model, axisCount);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("初始化控制器设备失败! ret = %1").arg(ret));
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText("连接控制器设备成功");
//     }
// }

// /*
// 移动到指定位置
// */
// void MainWindow::on_m_btn_move_clicked()
// {
//     int ID = ui->m_cbx_axis->currentIndex() + 1;
//     float disp = ui->m_spin_abs_position->text().toFloat();

//     // 设置move的绝对位置
//     int ret = ::setAbsoluteDisp(m_handle, ID, disp);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置绝对位置失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置绝对位置成功， disp: %1").arg(disp));
//     }

//     // move运行
//     ret = ::moveEmcvx(m_handle, ID, MOVE_CODE_MOVE);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("move运行失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("move运行成功"));
//     }
// }

// /*
// 向左步进
// */
// void MainWindow::on_c_btn_jogLeft_clicked()
// {
//     int ID = ui->m_cbx_axis->currentIndex() + 1;
//     float step = ui->m_spinx_jogStep->text().toFloat();

//     // 设置JOG运行的步长
//     int ret = ::setJogStep(m_handle, ID, step);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长成功， step: %1").arg(step));
//     }

//     // 设置JOG运行的次数
//     int time = ui->m_spin_times->text().toInt();
//     ret = ::setJogTime(m_handle, ID, time);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数成功， time: %1").arg(time));
//     }

//     // 设置JOG运行时每一步的停留延时时间
//     int delay = ui->m_spin_jogDelayTimes->text().toInt();
//     ret = ::setJogDelay(m_handle, ID, delay);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间成功， delay: %1").arg(delay));
//     }

//     // 向负方向jog运行
//     ret = ::moveEmcvx(m_handle, ID, MOVE_CODE_JOG_L);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("jog运行失败! ret: %1").arg(ret));
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("jog运行成功"));
//     }
// }

// /*
// 向右步进
// */
// void MainWindow::on_c_btn_jogRight_clicked()
// {
//     int ID = ui->m_cbx_axis->currentIndex() + 1;
//     float step = ui->m_spinx_jogStep->text().toFloat();

//     // 设置JOG运行的步长
//     int ret = ::setJogStep(m_handle, ID, step);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的步长成功， step: %1").arg(step));
//     }

//     // 设置JOG运行的次数
//     int time = ui->m_spin_times->text().toInt();
//     ret = ::setJogTime(m_handle, ID, time);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG运行的次数成功， time: %1").arg(time));
//     }

//     // 设置JOG运行时每一步的停留延时时间
//     int delay = ui->m_spin_jogDelayTimes->text().toInt();
//     ret = ::setJogDelay(m_handle, ID, delay);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间失败! ret: %1").arg(ret));
//         return;
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("设置JOG延时时间成功， delay: %1").arg(delay));
//     }

//     // 向正方向jog运行
//     ret = ::moveEmcvx(m_handle, ID, MOVE_CODE_JOG_R);
//     if (ret < 0)
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("jog运行失败! ret: %1").arg(ret));
//     }
//     else
//     {
//         // ui->c_PlainText_msg->appendPlainText(QString("jog运行成功"));
//     }
// }



// void MainWindow::on_c_spinbox_setJogStep_valueChanged(int arg1)
// {

// }

// /*
// 获取设置的当前位置
// GetCurrentPos
// */

/*
刷新位移台设备
*/
void MainWindow::on_m_btn_freshMover_clicked()
{

    //1.检测是否处于连接状态
    //2.断开连接状态
    //3.重新初始化
    this->initMover();
    statusBar()->showMessage("刷新位移台列表成功!");
}


void MainWindow::on_m_btn_connect_mover_clicked()
{
    //1.初始化设备参数
    QByteArray nameArray = ui->m_cbx_sirial_list->currentText().toLocal8Bit();
    char *name = nameArray.data();

    //2.判断设备是否连接
    int ret = ::isOpen(name);
    // qDebug() << ret;
    if(ret <= 0){
        //未打开
        m_handle = ::openEmcvx(name);            // 打开串口
        if (m_handle <= 0)
        {
            statusBar()->showMessage("打开串口失败!");
            ui->m_btn_connect_mover->setText("连接");
            return;
        }
        else
        {
            // 获取当前设备支持的轴数量
            int axisNum = ::getDeviceCode(m_handle);

            // 初始化轴设备
            int ID = 1;
            QByteArray modelArray = ui->m_cbx_modelName->currentText().toLocal8Bit();
            char *model = modelArray.data();
            int axisCount = ::getDeviceCode(m_handle);

            int ret = ::initAxis(m_handle, ID, model, axisCount);
            if (ret < 0)
            {
                ::closeEmcvx(m_handle);
                statusBar()->showMessage("初始化位移台失败!");
            }
            else
            {
                statusBar()->showMessage("连接位移台成功!");

                ui->m_btn_connect_mover->setText("断开");

            }

        }
    }else{
        // 关闭设备
        if( !::closeEmcvx(m_handle)){
            // qDebug() << m_handle;
            statusBar()->showMessage("断开位移台成功!");
            ui->m_btn_connect_mover->setText("连接");
        }
    }
}


void MainWindow::getMoverCurrentPos(){
    int *ok = nullptr;
    ::GetCurrentPos(m_handle,1,ok);
    ui->m_lcdNumber->display(*ok);
}


void MainWindow::on_m_btn_rest_clicked()
{
    //设置位移台复位

}


void MainWindow::on_m_btn_move_clicked()
{
    float disp = ui->m_spin_abs_position->text().toFloat();
    // 设置move的绝对位置
    int ret = ::setAbsoluteDisp(m_handle, 1, disp);
    if (ret < 0)
    {
        // ui->c_PlainText_msg->appendPlainText(QString("设置绝对位置失败! ret: %1").arg(ret));
        statusBar()->showMessage("设置绝对位置失败!");
        return;
    }

    // move运行
    ret = ::moveEmcvx(m_handle, 1, MOVE_CODE_MOVE);
    if (ret < 0)
    {
        // ui->c_PlainText_msg->appendPlainText(QString("move运行失败! ret: %1").arg(ret));
        statusBar()->showMessage("move运行失败!");
        return;
    }
    else
    {
        // ui->c_PlainText_msg->appendPlainText(QString("move运行成功"));
        statusBar()->showMessage("move运行成功!");
    }
}


void MainWindow::on_m_spin_abs_position_valueChanged(double arg1)
{
    // move运行
    ::setAbsoluteDisp(m_handle, 1, arg1);
    ::moveEmcvx(m_handle, 1, MOVE_CODE_MOVE);
}

