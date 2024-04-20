#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 获取串口列表
    char serialList[1024] = {0};
    int ret = ::listPorts(serialList, 1024);
    if (ret < 0)
    {
        qDebug() << "获取串口列表失败!";
        // ui->label_msg->setText("获取串口列表失败!");
    }

    // 获取位移台型号名称列表
    char modelsList[1024] = {0};
    ret = ::getAllModels(modelsList, 1024);
    if (ret < 0)
    {
        qDebug() << ("获取位移台型号名称列表失败!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
