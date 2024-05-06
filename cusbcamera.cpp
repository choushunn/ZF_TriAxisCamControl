

#include "cusbcamera.h"

#include <QCameraDevice>
#include <QMediaDevices>


CUSBCamera::CUSBCamera(){

}

CUSBCamera::~CUSBCamera(){
    close();
}

bool CUSBCamera::isOpened() const {
    // 检查是否已经打开
    return m_capture.isOpened();
}

int CUSBCamera::open(){
    //TODO:未实现

    return 0;
}

int CUSBCamera::open(int index) {
    m_index = index;
    // 打开
    if (!isOpened() && m_capture.open(m_index, cv::CAP_DSHOW)) {
        qDebug() << "CUSBCamera:打开成功！" << index;
        return 200;
    }
    return isOpened();
}

void CUSBCamera::close(){
    // 关闭
    m_capture.release();
    qDebug() << "CUSBCamera:关闭成功！";
}

bool CUSBCamera::read(cv::Mat& frame) {
    // 读取帧
    if (isOpened()) {
        return m_capture.read(frame);
    }
    return false;
}

void CUSBCamera::getCameraList(std::vector<std::string> &camera_list)
{
    // 获取USB相机列表
    QList<QCameraDevice> cameraList = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameraList) {
        camera_list.push_back(cameraDevice.description().toStdString());
    }
}

void CUSBCamera::saveImage()
{

}




