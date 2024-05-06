#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->m_btn_absStart->setDisabled(true);
    ui->m_btn_startJog->setDisabled(true);
    ui->m_chx_autoCapture->setDisabled(true);
    // 初始化位移台
    this->initMover();

    // 初始化UVC
    this->initUVC();
    m_readTimer = new QTimer();
    m_readTimer->setInterval(int(1000/30));
    m_readTimer->start();

    m_moverTimer = new QTimer();
    m_moverTimer->start(200);
    connect(m_moverTimer, &QTimer::timeout, this, &MainWindow::getMoverCurrentPos);
    this->setWindowIcon(QIcon(":/img/favicon.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_usbCap;
    delete m_takeTimer;
    delete m_readTimer;
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
    m_usbCap = new CUSBCamera();
    ui->m_cbx_cameraList->clear();
    // 将摄像头列表添加到QComboBox中
    std::vector <std::string> camera_list;
    m_usbCap->getCameraList(camera_list);
    for (const std::string &camera: camera_list) {
        ui->m_cbx_cameraList->addItem(camera.c_str());
    }
    m_camIndex = ui->m_cbx_cameraList->currentIndex();
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
                // 连接成功后，先执行复位
                this->on_m_btn_rest_clicked();
                statusBar()->showMessage("连接位移台成功!");
                ui->m_btn_absStart->setDisabled(false);
                ui->m_btn_startJog->setDisabled(false);
                ui->m_chx_autoCapture->setDisabled(false);
                ui->m_btn_connect_mover->setText("断开");
            }

        }
    }else{
        // 关闭设备
        if( !::closeEmcvx(m_handle)){
            // qDebug() << m_handle;
            statusBar()->showMessage("断开位移台成功!");
            ui->m_btn_connect_mover->setText("连接");
            ui->m_btn_absStart->setDisabled(true);
            ui->m_btn_startJog->setDisabled(true);
            ui->m_chx_autoCapture->setDisabled(true);
        }
    }
}


double MainWindow::getMoverCurrentPos(){
    // 获取位移台当前位置
    int *ok = nullptr;
    double pos = ::GetCurrentPos(m_handle, 1, ok);
    ui->m_lbl_currentPos->setText(QString::number(pos,'f', 3));
    return pos;
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


void MainWindow::on_m_spin_jogStep_valueChanged(int arg1)
{
    // 获取步长
    m_jogStep = arg1;
}


void MainWindow::on_m_spin_jogNum_valueChanged(int arg1)
{
    // 获取步数
    m_jogNum = arg1;
}


void MainWindow::on_m_spin_jogDelay_valueChanged(int arg1)
{
    // 获取延时间
    m_jogDelay = arg1;
}


void  MainWindow::takePhoto()
{
    QString folderPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    folderPath += "/outputs"; // 将 "output" 文件夹添加到文档目录中

    QDir folder(folderPath);

    // 检查文件夹是否存在，如果不存在则创建
    if (!folder.exists()) {
        if (folder.mkpath(".")) {
            qDebug() << "Folder created: " << folderPath;
        } else {
            qDebug() << "Failed to create folder: " << folderPath;
            return; // 在创建文件夹失败时进行错误处理
        }
    }

    QString filename = folder.filePath("spring_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz") + ".jpg");

    cv::Mat frame1 = frame;
    cv::cvtColor(frame1, frame1, cv::COLOR_BGR2RGB);
    cv::imwrite(filename.toStdString(), frame1);

    qDebug() << "Photo saved: " << filename;
}


void MainWindow::on_m_btn_startJog_clicked()
{
    // 重新获取
    m_jogNum = ui->m_spin_jogNum->value();
    m_jogStep = ui->m_spin_jogStep->value();
    m_jogDelay= ui->m_spin_jogDelay->value();



    //步进移动
    // qDebug() << m_jogStep*0.001 << m_jogDelay << m_jogNum ;
    double step = abs(m_jogStep*0.001);

    // 判断当前设置是否会超出量程
    double currentPos = abs(this->getMoverCurrentPos());
    double totalPos = step * m_jogNum + currentPos;
    // qDebug() << totalPos;
    if(totalPos>15){
        statusBar()->showMessage("当前步进参数超出量程,请重新设置!");
        // 弹出提示框
        QMessageBox::warning(NULL, "警告", "当前步进参数超出量程,请重新设置!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }


    // 设置JOG运行的步长
    int ret = ::setJogStep(m_handle, 1, step);
    if (ret < 0)
    {
        qDebug() << "设置JOG运行的步长失败";
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
    if(ui->m_chx_autoCapture->isChecked() && m_usbCap->isOpened()){
        // 设置间隔定时器，每隔一段时间触发拍照函数，直到次数结束
        m_takeTimer = new QTimer();
        m_takeTimer->setInterval(m_jogDelay);
        // 定时器的timeout()时
        QObject::connect(m_takeTimer, &QTimer::timeout, this, &MainWindow::takePhoto);
        // 开始定时器
        m_takeTimer->start(m_jogDelay);
        // 每次定时器触发时，执行次数加1，达到指定次数后停止定时器
        QObject::connect(m_takeTimer, &QTimer::timeout, [&]() {
            m_jogNum--;
            if (m_jogNum <= 0) {
                qDebug() << "Finished!";
                m_takeTimer->stop(); // 停止定时器
                // delete timer;
            }
        });
    }
    // m_jogDelay 间隔时间
    // m_jogNum  执行次数
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
    double arg1 = ui->m_spin_abs_position->value();
    int ret = ::setAbsoluteDisp(m_handle, 1, arg1);
    if (ret < 0)
    {
        statusBar()->showMessage("设置绝对位置失败!");
        return;
    }
    ret = ::moveEmcvx(m_handle, 1, MOVE_CODE_MOVE);
    if (ret < 0)
    {
        statusBar()->showMessage("move运行失败!");
        return;
    }
    else
    {
        getMoverCurrentPos();
        statusBar()->showMessage("move运行成功!");
    }
}


void MainWindow::showCameraFrame()
{
    m_usbCap->read(frame);
    if (frame.empty()) {
        return;
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    // 收到保存图像的信号，就保存图像

    // cv::resize(frame, frame, cv::Size(640, 480));
    // // 显示处理后的图像
    QImage qimage(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimage);
    ui->m_lbl_video->setPixmap(pixmap);
}

void MainWindow::on_m_btn_openCamera_clicked()
{
    // 打开相机
    if (m_usbCap->isOpened()) {
        if(m_readTimer->isActive()){
            m_readTimer->stop();
        };
        m_usbCap->close();
        ui->m_lbl_video->clear();
        qDebug() << "MainWindow:Camera closed successfully!";
        ui->m_btn_openCamera->setText("打开");
    } else {
        // 根据索引打开相机
        bool success = m_usbCap->open(m_camIndex);
        if (success) {
            m_readTimer->setInterval(int(1000/30));
            m_readTimer->start();
            qDebug() << "MainWindow:Camera opened successfully!";
            ui->m_btn_openCamera->setText("关闭");
        } else {
            // QMessageBox::critical(this, "Error", "Failed to open camera!");
        }
    }

    // 相机读取图像
    connect(m_readTimer, &QTimer::timeout, this, &MainWindow::showCameraFrame);
}


void MainWindow::on_m_btn_snap_clicked()
{
    // 拍照
    this->takePhoto();
}


void MainWindow::on_m_btn_stop_clicked()
{
    // 停止位移台运动
    ::moveEmcvx(m_handle, 1, MOVE_CODE_STOP);
}




void MainWindow::on_m_btn_record_clicked()
{
    getMoverCurrentPos();
}

