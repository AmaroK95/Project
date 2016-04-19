#include "facedetector.h"
#include <QtCore>
#include <vector>
#include <QFileDialog>

#define DEFAULT_CASCADE_PATH "cascades\\lbpcascade_frontalface.xml"

FaceDetector::FaceDetector(QObject *parent): QObject(parent)
{
    processTime=new QElapsedTimer();
    webcam.open(0);
    if(!webcam.isOpened())
    {
        emit statusChanged("Web camera error!");
    }

    //load cascade classifier
    loadCascadeClassifier(DEFAULT_CASCADE_PATH);
    frames=0;
}


void FaceDetector::exitThread()
{
    emit finished();
}
void FaceDetector::loadCascadeClassifier(QString path)
{
    QString result=faceDetector.load(path.toStdString())? "Loaded new cascade classifier: ": "Cant load cascade classifier: ";
    emit statusChanged(result+path);
}
void FaceDetector::processFrame()
{
    processTime->start();
    webcam.read(inputFrame);

    cv::cvtColor(inputFrame,outputFrame,CV_BGR2RGB); //4 nanosec

    cv::Size size(320,240);
    cv::resize(inputFrame,inputFrame,size);

    cv::cvtColor(inputFrame,processedFrame,CV_BGR2GRAY); //4 nanosec

    detect(); //60 nanosec

    cv::putText(outputFrame,"Frames: "+QString::number(++frames).toStdString(),cv::Point(10,50),cv::FONT_HERSHEY_COMPLEX_SMALL,0.7,cv::Scalar(0, 255, 0));
    cv::putText(outputFrame,"Delay: "+QString::number(processTime->elapsed()).toStdString(),cv::Point(10,65),cv::FONT_HERSHEY_COMPLEX_SMALL,0.7,cv::Scalar(0, 255, 0));
    outputImage = QImage((uchar*)outputFrame.data,outputFrame.cols,outputFrame.rows,outputFrame.step,QImage::Format_RGB888);

    emit imageProcessed(outputImage);
}
void FaceDetector::detect()
{
    std::vector<cv::Rect> faceRects;
    double scalingFactor = 1.1;
    int minNeighbors = 5;
    int flags=0;

    faceDetector.detectMultiScale(processedFrame, faceRects, scalingFactor, minNeighbors, flags,  cv::Size(50, 50));

    std::vector<cv::Rect>::iterator it;
    cv::Rect face;
    for(it=faceRects.begin();it!=faceRects.end();it++) {
        face=*it;
        cv::rectangle(outputFrame,face.tl()*2,face.br()*2,cv::Scalar(255, 0, 0));

    }

}

/*GPU
void FaceDetector::DetectFace()
{
    cv::gpu::CascadeClassifier_GPU cascade_gpu("C:\\workspace\\Qt\\OpenCV\\FR\\lbpcascade_frontalface.xml");
    cv::gpu::GpuMat image_gpu(processedFrame);

    cv::gpu::GpuMat objbuf;
    int detections_number = cascade_gpu.detectMultiScale( image_gpu,
              objbuf, 1.4, 4);

    cv::Mat obj_host;
    // download only detected number of rectangles
    objbuf.colRange(0, detections_number).download(obj_host);

    cv::Rect *faces = obj_host.ptr<cv::Rect>();
    for(int i = 0; i < detections_number; ++i)
       cv::rectangle(outputFrame, faces[i], cv::Scalar(255));
}*/
