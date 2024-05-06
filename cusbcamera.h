#ifndef CUSBCAMERA_H
#define CUSBCAMERA_H

#include <opencv2/opencv.hpp>
#include "QDebug"

class CUSBCamera
{
public:
//    static CUSBCamera& getInstance();
    CUSBCamera();
    ~CUSBCamera();
    bool isOpened() const ;
    int open()  ;
    int open(int index) ;
    void close() ;
    bool read(cv::Mat& frame) ;
    void getCameraList(std::vector<std::string> &camera_list) ;
    void saveImage() ;
private:
//    CUSBCamera();
//    CUSBCamera(const CUSBCamera&) = delete;
//    CUSBCamera& operator=(const CUSBCamera&) = delete;
//    ~CUSBCamera() = default;

    int m_index = 0;
    cv::VideoCapture m_capture;
};

#endif // CUSBCAMERA_H
