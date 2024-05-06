#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化位移台
    this->initMover();

    // 初始化UVC
    this->initUVC();
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

void MainWindow::initUVC()
{
    // // 获取可用摄像头列表
    // QList<QString> cameraList;
    // int cameraIndex = 0;
    // while (true) {
    //     cv::VideoCapture capture(cameraIndex);
    //     if (!capture.isOpened()) {
    //         break;
    //     }
    //     else {
    //         QString cameraName = "Camera " + QString::number(cameraIndex);
    //         cameraList.append(cameraName);
    //         capture.release();
    //         cameraIndex++;
    //     }
    // }

    // // 将摄像头列表添加到QComboBox中
    // ui->m_cbx_cameraList->addItems(cameraList);

}

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
            // int axisNum = ::getDeviceCode(m_handle);

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

}


void MainWindow::on_m_btn_rest_clicked()
{
    //设置位移台复位

    // move运行
    int ret = ::moveEmcvx(m_handle, 1, MOVE_CODE_RESTORE);
    if (ret < 0)
    {
        statusBar()->showMessage("复位失败!");
        return;
    }
    else
    {
        statusBar()->showMessage("复位运行成功!");
    }
}




void MainWindow::on_m_spin_abs_position_valueChanged(double arg1)
{

}


void MainWindow::on_m_spinx_jogStep_valueChanged(int arg1)
{
    // 获取步长
    m_jogStep = arg1;
}


void MainWindow::on_m_spin_jogNumber_valueChanged(int arg1)
{
    // 获取步数
    m_jogNum = arg1;
}


void MainWindow::on_m_spin_jogDelayTimes_valueChanged(int arg1)
{
    // 获取延时间
    m_jogDelay = arg1;
}




void MainWindow::on_m_btn_stopJog_clicked()
{
    //步进移动
    qDebug() << m_jogStep*0.001 << m_jogDelay << m_jogNum ;
    double step = abs(m_jogStep*0.001);
    // 设置JOG运行的步长
    int ret = ::setJogStep(m_handle, 1, step);
    if (ret < 0)
    {
        return;
    }
    else
    {
        qDebug() << "设置JOG运行的步长成功，";
    }

    // 设置JOG运行的次数

    ret = ::setJogTime(m_handle, 1, m_jogNum);
    if (ret < 0)
    {

        qDebug() << "设置JOG运行的次数失败";
        return;
    }
    else
    {
        qDebug() << "设置JOG运行的次数成功，";
    }

    // 设置JOG运行时每一步的停留延时时间
    ret = ::setJogDelay(m_handle, 1, m_jogDelay);
    if (ret < 0)
    {
        qDebug() << "设置JOG延时时间失败";
        return;
    }
    else
    {
        qDebug() << "设置JOG延时时间成功";

    }
    // 如果
    ui->m_chx_autoCapture->isChecked();

    if(m_jogStep < 0){
        // 向左移动
        // 向负方向 jog 运行
        ret = ::moveEmcvx(m_handle, 1, MOVE_CODE_JOG_L);
        if (ret < 0)
        {
            qDebug() << "jog运行失败";

        }
        else
        {
            qDebug() << "jog运行成功";

        }
    }else{
        // 向右移动
        // 向正方向 jog 运行
        ret = ::moveEmcvx(m_handle, 1, MOVE_CODE_JOG_R);
        if (ret < 0)
        {
            qDebug() << "jog运行失败";

        }
        else
        {
            qDebug() << "jog运行成功";

        }
    }
}


void MainWindow::on_m_btn_absStart_clicked()
{
    // move运行
    int arg1 = ui->m_spin_abs_position->value();
    int ret = ::setAbsoluteDisp(m_handle, 1, arg1);
    if (ret < 0)
    {
        statusBar()->showMessage("设置绝对位置失败!");
        return;
    }
    ::moveEmcvx(m_handle, 1, MOVE_CODE_MOVE);
    if (ret < 0)
    {
        statusBar()->showMessage("move运行失败!");
        return;
    }
    else
    {
        statusBar()->showMessage("move运行成功!");
    }
}



void MainWindow::on_m_btn_openCamera_clicked()
{

}

